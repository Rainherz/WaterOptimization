#include "pso.h"
#include <iostream>
#include <random>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <iomanip>

PSO_DistribucionAgua::PSO_DistribucionAgua(int maxIteraciones)
    : mejorValorGlobal(std::numeric_limits<double>::infinity()),
    iterations(maxIteraciones) {    // Inicializar datos de distritos de Arequipa
    distritos = {
        {"Alto Selva Alegre", 85870, 140, 11500},
        {"Cayma", 91935, 145, 12000},
        {"Cercado", 55437, 150, 8200},
        {"Cerro Colorado", 197954, 160, 25000},
        {"Jacobo Hunter", 48326, 135, 6800},
        {"Mariano Melgar", 52667, 140, 7500},
        {"Miraflores", 48677, 135, 6900},
        {"Paucarpata", 131346, 155, 17000},
        {"Sachaca", 24225, 130, 3500},
        {"Socabaya", 75351, 140, 10000},
        {"Tiabaya", 16191, 125, 2500},
        {"Yanahuara", 25483, 130, 3800},
        {"Jose Luis Bustamante y Rivero", 76711, 145, 10500}
    };

    inicializarParticulas();
}

// Agregar el método setIterations
void PSO_DistribucionAgua::setIteraciones(int iteraciones) {
    iterations = iteraciones;
}

void PSO_DistribucionAgua::inicializarParticulas() {
    int dimensiones = distritos.size();
    particulas.resize(NUM_PARTICULAS, std::vector<double>(dimensiones));
    velocidades.resize(NUM_PARTICULAS, std::vector<double>(dimensiones));
    mejoresLocales.resize(NUM_PARTICULAS, std::vector<double>(dimensiones));
    mejoresValoresLocales.resize(NUM_PARTICULAS, std::numeric_limits<double>::infinity());
    mejorGlobal.resize(dimensiones);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.8, 1.2);

    for (int i = 0; i < NUM_PARTICULAS; ++i) {
        for (int j = 0; j < dimensiones; ++j) {
            particulas[i][j] = distritos[j].getDistribucionOriginal() * dis(gen);
            velocidades[i][j] = distritos[j].getDistribucionOriginal() * (dis(gen) - 1.0) * 0.1;
        }

        double fitness = evaluarFitness(particulas[i]);
        mejoresLocales[i] = particulas[i];
        mejoresValoresLocales[i] = fitness;

        if (fitness < mejorValorGlobal) {
            mejorValorGlobal = fitness;
            mejorGlobal = particulas[i];
        }
    }
}

double PSO_DistribucionAgua::evaluarFitness(const std::vector<double> &distribucion) {
    double penalizacion = 0.0;
    double totalAgua = std::accumulate(distribucion.begin(), distribucion.end(), 0.0);

    for (size_t i = 0; i < distritos.size(); ++i) {
        if (distribucion[i] < distritos[i].getDemandaBase()) {
            penalizacion += pow(distritos[i].getDemandaBase() - distribucion[i], 2) * 10;
        }

        double proporcionPoblacion = static_cast<double>(distritos[i].getPoblacion()) / getTotalPoblacion();
        double proporcionAgua = distribucion[i] / totalAgua;
        penalizacion += pow(proporcionPoblacion - proporcionAgua, 2) * 1000;

        double cambioPorcentual = abs(distribucion[i] - distritos[i].getDistribucionOriginal()) / distritos[i].getDistribucionOriginal();
        if (cambioPorcentual > 0.3) {
            penalizacion += pow(cambioPorcentual - 0.3, 2) * 500;
        }
    }

    double totalOriginal = getTotalAguaOriginal();
    double cambioPorcentualTotal = abs(totalAgua - totalOriginal) / totalOriginal;
    if (cambioPorcentualTotal > 0.1) {
        penalizacion += pow(cambioPorcentualTotal - 0.1, 2) * 1000;
    }

    return penalizacion;
}

int PSO_DistribucionAgua::getTotalPoblacion() const {
    int total = 0;
    for (const auto &distrito : distritos) {
        total += distrito.getPoblacion();
    }
    return total;
}

double PSO_DistribucionAgua::getTotalAguaOriginal() const {
    double total = 0.0;
    for (const auto &distrito : distritos) {
        total += distrito.getDistribucionOriginal();
    }
    return total;
}

void PSO_DistribucionAgua::optimizar() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int iter = 0; iter < iterations; ++iter) {  // Usar iterations en lugar de MAX_ITERATIONS
        for (int i = 0; i < NUM_PARTICULAS; ++i) {
            for (size_t j = 0; j < distritos.size(); ++j) {
                double r1 = dis(gen);
                double r2 = dis(gen);

                velocidades[i][j] = W * velocidades[i][j] +
                                    C1 * r1 * (mejoresLocales[i][j] - particulas[i][j]) +
                                    C2 * r2 * (mejorGlobal[j] - particulas[i][j]);

                double velocidadMaxima = distritos[j].getDistribucionOriginal() * 0.1;
                velocidades[i][j] = std::clamp(velocidades[i][j], -velocidadMaxima, velocidadMaxima);

                particulas[i][j] += velocidades[i][j];
                particulas[i][j] = std::clamp(particulas[i][j], distritos[j].getDistribucionOriginal() * 0.5, distritos[j].getDistribucionOriginal() * 1.5);
            }

            double fitness = evaluarFitness(particulas[i]);
            if (fitness < mejoresValoresLocales[i]) {
                mejoresValoresLocales[i] = fitness;
                mejoresLocales[i] = particulas[i];

                if (fitness < mejorValorGlobal) {
                    mejorValorGlobal = fitness;
                    mejorGlobal = particulas[i];
                }
            }
        }

        // Actualizar la distribución optimizada para cada distrito
        for (size_t i = 0; i < distritos.size(); ++i) {
            distritos[i].setDistribucionOptima(mejorGlobal[i]);
        }

        if ((iter + 1) % 50 == 0) {
            std::cout << "Iteración " << (iter + 1) << ": Mejor valor global = " << mejorValorGlobal << std::endl;
        }
    }
}

void PSO_DistribucionAgua::mostrarResultados() {
    std::cout << "Resultados de la Optimización:\n";
    std::cout << std::setw(20) << "Distrito"
              << std::setw(15) << "Población"
              << std::setw(20) << "Distribución Actual"
              << std::setw(20) << "Distribución Optimizada"
              << std::setw(15) << "% Variación" << std::endl;

    for (size_t i = 0; i < distritos.size(); ++i) {
        double variacion = ((mejorGlobal[i] - distritos[i].getDistribucionOriginal()) / distritos[i].getDistribucionOriginal()) * 100;
        std::cout << std::setw(20) << distritos[i].getNombre()
                  << std::setw(15) << distritos[i].getPoblacion()
                  << std::setw(20) << distritos[i].getDistribucionOriginal()
                  << std::setw(20) << distritos[i].getDistribucionOptima()
                  << std::setw(15) << variacion << "%" << std::endl;
    }
}

const std::vector<Distrito>& PSO_DistribucionAgua::getDistritos() const {
    return distritos;
}

const std::vector<double>& PSO_DistribucionAgua::getMejorGlobal() const {
    return mejorGlobal;
}
