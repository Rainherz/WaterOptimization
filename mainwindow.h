#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTableWidget>
#include <QtCharts>
#include "pso.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onOptimizeClicked();
    void updateUIWithOptimizedData(const std::vector<Distrito>& distritos, const std::vector<double>& optimizado);
    void updatePieChart(const std::vector<Distrito>& distritos, const std::vector<double>& optimizado);
    void updateLegendTable(const std::vector<Distrito>& distritos, const std::vector<double>& optimizado);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *pieScene;
    void setupUI();
    void createPieChart();
    void setupLegendTable();
    std::vector<Distrito> getDistrictData();
    void assignDistrictDataToButtons();
    QString generateDistrictTooltip(const QString& districtName, double population, double demand, double distributionOriginal, double distributionOptimized);
    PSO_DistribucionAgua pso;
    void addTableRow(QTableWidget* table, int row, const Distrito& distrito, double percentage);
    void replaceChartInLayout(QChartView* chartView);
};
#endif // MAINWINDOW_H
