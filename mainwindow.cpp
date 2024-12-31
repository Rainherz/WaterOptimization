#include "mainwindow.h"
#include "popupdialog.h"
#include "pso.h"
#include "maputils.h"
#include "./ui_mainwindow.h"
#include <QPixmap>
#include <QPainter>
#include <QTableWidgetItem>
#include <QtCharts>
#include <QMessageBox>
#include <QToolTip>
#include <QMap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , pso(0)
{
    ui->setupUi(this);
    setupUI();

    connect(ui->optimizeButton, &QPushButton::clicked, this, &MainWindow::onOptimizeClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    createPieChart();
    setupLegendTable();
    assignDistrictDataToButtons();
}

void MainWindow::assignDistrictDataToButtons()
{
    auto distritos = getDistrictData();

    // Crear un mapa para acceso rápido a los distritos por nombre sin espacios
    QMap<QString, Distrito> districtMap;
    for (const auto& distrito : distritos) {
        QString districtNameWithoutSpaces = QString::fromStdString(distrito.getNombre());
        districtNameWithoutSpaces.remove(" ");
        districtMap[districtNameWithoutSpaces] = distrito;
    }

    // Iterar sobre todos los botones, asignando datos según el nombre del botón
    for (auto& button : findChildren<QPushButton*>()) {
        QString buttonName = button->objectName();

        // Buscar el distrito correspondiente al nombre del botón
        if (districtMap.contains(buttonName)) {
            auto distrito = districtMap[buttonName];

            // Generar el texto del tooltip con todos los datos
            QString tooltip = generateDistrictTooltip(
                QString::fromStdString(distrito.getNombre()),
                distrito.getPoblacion(),
                distrito.getDemandaBase(),
                distrito.getDistribucionOriginal(),
                distrito.getDistribucionOptima()
                );
            button->setToolTip(tooltip);
        }
    }
}

QString MainWindow::generateDistrictTooltip(const QString& districtName, double population, double demand, double distributionOriginal, double distributionOptimized)
{
    return QString("<b>Distrito:</b> %1<br>"
                   "<i>Población:</i> %2<br>"
                   "<i>Demanda Base:</i> %3 m³/día<br>"
                   "<i>Distribución Actual:</i> %4 m³/día<br>"
                   "<i>Distribución Optimizada:</i> %5 m³/día")
        .arg(districtName)
        .arg(population)
        .arg(demand, 0, 'f', 1)
        .arg(distributionOriginal, 0, 'f', 1)
        .arg(distributionOptimized, 0, 'f', 1);
}

std::vector<Distrito> MainWindow::getDistrictData() {
    return pso.getDistritos(); // Accede a los distritos desde la instancia de PSO_DistribucionAgua
}

void MainWindow::createPieChart() {
    // Obtiene los distritos
    auto distritos = getDistrictData();

    // Crear la serie de pie
    QChart *chart = new QChart();
    QPieSeries *series = new QPieSeries();

    // Calcular el total de distribución original
    double total = 0;
    for (const auto &distrito : distritos) {
        total += distrito.getDistribucionOriginal();
    }

    // Agregar rebanadas al gráfico
    int index = 0;
    for (const auto &distrito : distritos) {
        double percentage = (distrito.getDistribucionOriginal() / total) * 100.0;
        QPieSlice *slice = series->append(QString::fromStdString(distrito.getNombre()), percentage);

        // Establecer propiedades de la rebanada
        slice->setBrush(MapUtils::getDistrictColor(index++));

        // Hacer las rebanadas ligeramente explotar para visibilidad
        slice->setExploded(true);
        slice->setExplodeDistanceFactor(0.05);

        // Conectar las señales de hover
        connect(slice, &QPieSlice::hovered, [slice, distrito, percentage](bool show) {
            if (show) {
                QString tooltip = QString("%1\n%2 m³/día\n%3%")
                                      .arg(QString::fromStdString(distrito.getNombre()))
                                      .arg(distrito.getDistribucionOriginal(), 0, 'f', 1)
                                      .arg(percentage, 0, 'f', 1);
                QToolTip::showText(QCursor::pos(), tooltip);

                // Efecto de resaltado
                slice->setExplodeDistanceFactor(0.1);
                slice->setLabelVisible(true);
            } else {
                QToolTip::hideText();

                // Volver al estado normal
                slice->setExplodeDistanceFactor(0.05);
                slice->setLabelVisible(false);
            }
        });
    }

    // Configuración del gráfico
    chart->addSeries(series);
    chart->setTitle("Distribución Actual del Agua");
    chart->legend()->hide();

    // Establecer opciones de animación
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Crear vista de gráfico
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Habilitar seguimiento del mouse para efectos de hover
    chartView->setMouseTracking(true);

    // Reemplazar la vista gráfica actual con la nueva vista de gráfico
    QLayoutItem *item = ui->rightLayout->takeAt(0);
    delete item->widget();
    delete item;
    ui->rightLayout->insertWidget(0, chartView);
}

void MainWindow::replaceChartInLayout(QChartView* chartView) {
    QLayoutItem* item = ui->rightLayout->takeAt(0);
    delete item->widget();
    delete item;
    ui->rightLayout->insertWidget(0, chartView);
}

void MainWindow::setupLegendTable()
{
    auto distritos = getDistrictData();
    double total = std::accumulate(distritos.begin(), distritos.end(), 0.0, [](double sum, const Distrito& distrito) {
        return sum + distrito.getDistribucionOriginal();
    });

    QTableWidget* table = ui->legendTable;
    table->setColumnCount(5); // Aumentamos la cantidad de columnas
    table->setRowCount(distritos.size());

    QStringList headers = { "Distrito", "Población", "D. Actual (m³)", "D. Optimizada (m³)", "% Variación" };
    table->setHorizontalHeaderLabels(headers);

    // Agregar datos a la tabla
    for (size_t i = 0; i < distritos.size(); ++i) {
        double percentage = (distritos[i].getDistribucionOriginal() / total) * 100.0;
        addTableRow(table, i, distritos[i], percentage);
    }

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::addTableRow(QTableWidget* table, int row, const Distrito& distrito, double percentage) {
    QTableWidgetItem *nameItem = new QTableWidgetItem(QString::fromStdString(distrito.getNombre()));
    nameItem->setBackground(MapUtils::getDistrictColor(row));
    table->setItem(row, 0, nameItem);

    table->setItem(row, 1, new QTableWidgetItem(QString::number(distrito.getPoblacion())));
    table->setItem(row, 2, new QTableWidgetItem(QString::number(distrito.getDistribucionOriginal(), 'f', 1)));
    table->setItem(row, 3, new QTableWidgetItem(QString::number(distrito.getDistribucionOptima(), 'f', 1)));
    table->setItem(row, 4, new QTableWidgetItem(QString::number(percentage, 'f', 1) + "%"));
}

void MainWindow::onOptimizeClicked()
{
    PopupDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        int iterations = dialog.getIterations();

        pso.setIteraciones(iterations);
        pso.optimizar();
        assignDistrictDataToButtons();

        auto distritos = pso.getDistritos();
        auto resultados = pso.getMejorGlobal();

        updateUIWithOptimizedData(distritos, resultados);
    }
}

void MainWindow::updateUIWithOptimizedData(const std::vector<Distrito>& distritos, const std::vector<double>& optimizado)
{
    updatePieChart(distritos, optimizado);
    updateLegendTable(distritos, optimizado);
}

void MainWindow::updatePieChart(const std::vector<Distrito>& distritos, const std::vector<double>& optimizado)
{
    QChart *chart = new QChart();
    QPieSeries *series = new QPieSeries();

    // Calcular el total para porcentajes
    double total = std::accumulate(optimizado.begin(), optimizado.end(), 0.0);

    // Agregar datos al gráfico
    for (size_t i = 0; i < distritos.size(); ++i) {
        double percentage = (optimizado[i] / total) * 100.0;
        QPieSlice *slice = series->append(QString::fromStdString(distritos[i].getNombre()), percentage);

        // Configurar propiedades del slice
        slice->setBrush(MapUtils::getDistrictColor(i));
        slice->setExploded(true);
        slice->setExplodeDistanceFactor(0.05);

        // Conectar señal de hover para mostrar detalles
        connect(slice, &QPieSlice::hovered, [slice, distritos, optimizado, i, percentage](bool show) {
            if (show) {
                QString tooltip = QString("%1\n%2 m³/día\n%3%")
                                      .arg(QString::fromStdString(distritos[i].getNombre()))
                                      .arg(optimizado[i], 0, 'f', 1)
                                      .arg(percentage, 0, 'f', 1);
                QToolTip::showText(QCursor::pos(), tooltip);

                slice->setExplodeDistanceFactor(0.1);
                slice->setLabelVisible(true);
            } else {
                QToolTip::hideText();

                slice->setExplodeDistanceFactor(0.05);
                slice->setLabelVisible(false);
            }
        });
    }

    // Configurar el gráfico
    chart->addSeries(series);
    chart->setTitle("Distribución Optimizada del Agua");
    chart->legend()->hide();
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Crear el visor del gráfico
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMouseTracking(true);

    // Reemplazar el gráfico actual en el diseño
    QLayoutItem* item = ui->rightLayout->takeAt(0);
    delete item->widget();
    delete item;
    ui->rightLayout->insertWidget(0, chartView);
}

void MainWindow::updateLegendTable(const std::vector<Distrito>& distritos, const std::vector<double>& optimizado)
{
    ui->legendTable->setRowCount(distritos.size());

    for (size_t i = 0; i < distritos.size(); ++i) {
        double variacion = ((optimizado[i] - distritos[i].getDistribucionOriginal()) / distritos[i].getDistribucionOriginal()) * 100;
        addTableRow(ui->legendTable, i, distritos[i], variacion);
    }

    ui->legendTable->resizeColumnsToContents();
}
