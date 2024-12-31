#ifndef PSO_H
#define PSO_H

#include <vector>
#include "distrito.h"

// Clase que implementa el algoritmo PSO para la optimización de distribución de agua
class PSO_DistribucionAgua {
private:
    std::vector<Distrito> distritos;
    std::vector<std::vector<double>> particulas;
    std::vector<std::vector<double>> velocidades;
    std::vector<std::vector<double>> mejoresLocales;
    std::vector<double> mejoresValoresLocales;
    double mejorValorGlobal;
    std::vector<double> mejorGlobal;
    int iterations;

    // Parámetros del PSO
    const int NUM_PARTICULAS = 50;
    const double W = 0.729;     // Factor de inercia
    const double C1 = 1.49445;  // Factor cognitivo
    const double C2 = 1.49445;  // Factor social

    // Métodos internos
    void inicializarParticulas();
    double evaluarFitness(const std::vector<double> &distribucion);
    int getTotalPoblacion() const;
    double getTotalAguaOriginal() const;

public:
    PSO_DistribucionAgua();

    // Métodos principales
    void optimizar();
    void mostrarResultados();

    // Métodos auxiliares
    const std::vector<Distrito>& getDistritos() const;
    const std::vector<double>& getMejorGlobal() const;
    explicit PSO_DistribucionAgua(int maxIteraciones = 200); // Constructor con iteraciones configurables
    void setIteraciones(int iteraciones);
};

#endif // PSO_H
