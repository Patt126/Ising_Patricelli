/**
 * @file DomainDecomposition.h
 * @brief Header file for the DomainDecomposition class.
 */

#ifndef MY_PROJECT_DOMAIN_DECOMPOSITION_H
#define MY_PROJECT_DOMAIN_DECOMPOSITION_H

#include <array>
#include <vector>
#include <memory>
#include <random>
#include "AbstractMonteCarloSimulation.h"
#include "SquareLattice.h"

/**
 * @brief Class representing a Monte Carlo simulation using domain decomposition.
 *
 * This class inherits from AbstractMonteCarloSimulation and provides specific
 * implementations for the simulation steps using domain decomposition. The simulation
 * is performed by dividing the lattice into subdomains, with each subdomain simulated
 * concurrently on a separate thread, menaging atomically boundary sites.
 * Parallelization is performed using OpenMP, in particular creating task corresponding
 * to job performed in each block. This help when introducing convergence criteria.
 */
class DomainDecomposition : public AbstractMonteCarloSimulation {
public:
    /**
     * @brief Constructor for the DomainDecomposition class.
     *
     * @param interactionStrength The spin interaction strength parameter.
     * @param latticeSize The size of the square lattice.
     * @param NUMTHREAD The number of threads to use in the simulation.
     * @param T_MIN The minimum temperature.
     * @param T_MAX The maximum temperature.
     * @param T_STEP The temperature step size.
     * @param IT The number of Monte Carlo steps.
     */
    DomainDecomposition(float interactionStrength, int latticeSize, int NUMTHREAD, float T_MIN, float T_MAX, float T_STEP, long int IT);

    /**
     * @brief Perform the simulation of a phase transition using domain decomposition.
     */
    void simulate_phase_transition() override;


    /**
     * @brief Store the results of the simulation to a file.
     */
    void store_results_to_file() const override;



   
    /**
     * @brief Flip a spin at a given lattice site during the simulation.
     *
     * @param lattice Reference to the lattice vector.
     * @param prob Probability array for the simulation.
     * @param site Index of the lattice site to flip.
     * @param magnetization Reference to the magnetization variable.
     * @param energy Reference to the energy variable.
     * @param rng_private Random number generator.
     */
    void flip(std::vector<int>& lattice, std::array<float, 2>& prob, const int& site, int& magnetization, int& energy, std::mt19937& rng_private);

    /**
     * @brief Atomically flip a spin at a given lattice site during the simulation.
     * used to flip boundary sites.
     *
     * @param lattice Reference to the lattice vector.
     * @param prob Probability array for the simulation.
     * @param site Index of the lattice site to flip.
     * @param magnetization Reference to the magnetization variable.
     * @param energy Reference to the energy variable.
     */
    void atomic_flip(std::vector<int>& lattice, std::array<float, 2>& prob, const int& site, int& magnetization, int& energy, std::mt19937& rng_private);

    /**
     * @brief Simulate the step of a thread block in the Monte Carlo simulation.
     * Each thread perform cuncurrently IT/NUMTHREAD flip.
     * @param prob Probability array for the simulation.
     * @param lattice Reference to the lattice vector.
     * @param magnetization Reference to the magnetization variable.
     * @param energy Reference to the energy variable.
     * @param offset Offset for the starting point of each block.
     */
    void simulate_step(std::array<float, 2> prob, std::vector<int>& lattice, int& magnetization, int& energy, const int& offset) override;
    /**
     * @brief Set the block width for the domain decomposition.
     *
     * @return The block size.
     */
    int set_block_size();

private:
    SquareLattice lattice;  /**< Square lattice used in the simulation. */
    std::vector<float> EnergyResults;  /**< Vector to store energy results. */
    std::vector<float> MagnetizationResults;  /**< Vector to store magnetization results. */
    std::vector<float> Temperatures;  /**< Vector to store temperature visited. */
    std::vector<int> ThreadStart;  /**< Vector to store Monte Carlo steps. */
    const float T_MIN;
    const float T_MAX;
    const float T_STEP;
    const int L;
    const int N;
    const long int IT;
    const int NUMTHREAD;
    int A;
    std::uniform_real_distribution<double> dist;  /**< Uniform distribution in [0, 1). */
};

#endif // MY_PROJECT_DOMAIN_DECOMPOSITION_H
