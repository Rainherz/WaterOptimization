#ifndef DISTRITO_H
#define DISTRITO_H

#include <string>

class Distrito
{
private:
    std::string nombre;
    int poblacion;
    double demandaBase;           // metros cúbicos por día
    double distribucionOriginal;  // distribución actual
    double distribucionOptima;    // distribución optimizada

public:
    // Constructor
    Distrito(const std::string& n = "", int p = 0, double d = 0.0, double o = 0.0, double opt = 0.0);

    // Getters
    const std::string& getNombre() const;
    int getPoblacion() const;
    double getDemandaBase() const;
    double getDistribucionOriginal() const;
    double getDistribucionOptima() const;  // Getter para la nueva propiedad

    // Setters
    void setNombre(const std::string& n);
    void setPoblacion(int p);
    void setDemandaBase(double d);
    void setDistribucionOriginal(double o);
    void setDistribucionOptima(double opt);  // Setter para la nueva propiedad

    // Métodos adicionales
    double calcularDemandaPorPersona() const;  // Retorna demandaBase / población
    void imprimirInfo() const;                // Muestra información del distrito
};

#endif // DISTRITO_H
