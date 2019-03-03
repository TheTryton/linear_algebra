#pragma once

#include "../matrix/matrix.hpp"
#include "../vector/vector.hpp"
#include "../common_matrix_vector_operations.inl"

NAMESPACE_LINEAR_ALGEBRA_BEGIN

///<summary>
/// solves linear equations system (N equations, M variables)
/// <para>*** RESULTS ***</para>
/// <para>if equation system is contradictory/indeterminate returns nullopt</para>
/// <para>if equation system is determinate returns solution</para>
///</summary>
/// <param name="coefficents"> linear equation system coefficents represented in NxM matrix </param>
/// <param name="constant_terms"> linear equation system constant terms </param>
template<class T, size_t N, size_t M>
std::optional<equation_system_solution<T,M>> solve_equation_system(const matrix<T, N, M>& coefficents, const vector<T, N>& constant_terms)
{
    auto solve_with_gaussian_elimination = [&]()->std::optional<equation_system_solution<T, M>> {
        auto copy = coefficents;
        auto copy_terms = constant_terms;

        constexpr size_t D = N > M ? M : N;

        //using gaussian elimination to obtain matrix in row
        //echelon form

        size_t column_shift = 0;
        for (size_t diag = 0; diag < D; diag++)
        {
            bool no_non_zero = false;

            while (equal(copy[diag][diag + column_shift], static_cast<T>(0)) && !no_non_zero)
            {
                size_t non_zero_row = diag;

                for (size_t f_row = diag + 1; f_row < N; f_row++)
                {
                    if (!equal(copy[f_row][diag + column_shift], static_cast<T>(0)))
                    {
                        non_zero_row = f_row;
                        break;
                    }
                }

                if (non_zero_row != diag)
                {
                    copy.swap_rows(copy, diag, non_zero_row, diag + column_shift);

                    std::swap(copy_terms[diag], copy_terms[non_zero_row]);
                }
                else
                {
                    column_shift++;
                    if (diag + column_shift >= M)
                    {
                        // equation system is indeterminate
                        no_non_zero = true;
                        break;
                    }
                }
            }

            if (no_non_zero)
                break;

            T factor = copy[diag][diag + column_shift];
            if(!equal(factor,static_cast<T>(0)))
            {
                for (size_t column = diag + column_shift; column < M; column++)
                {
                    copy[diag][column] /= factor;
                }
                copy_terms[diag] /= factor;
            }
            

            for (size_t reduced_row = 0; reduced_row < diag; reduced_row++)
            {
                if (!equal(copy._mat[diag][diag + column_shift], static_cast<T>(0)))
                {
                    T factor = copy._mat[reduced_row][diag + column_shift] / copy._mat[diag][diag + column_shift];
                    for (size_t column = diag + column_shift; column < M; column++)
                    {
                        copy._mat[reduced_row][column] -= copy._mat[diag][column] * factor;
                    }

                    copy_terms[reduced_row] -= copy_terms[diag] * factor;
                }
            }

            for (size_t reduced_row = diag + 1; reduced_row < N; reduced_row++)
            {
                if (!equal(copy._mat[diag][diag + column_shift], static_cast<T>(0)))
                {
                    T factor = copy._mat[reduced_row][diag + column_shift] / copy._mat[diag][diag + column_shift];

                    for (size_t column = diag + column_shift; column < M; column++)
                    {
                        copy._mat[reduced_row][column] -= copy._mat[diag][column] * factor;
                    }

                    copy_terms[reduced_row] -= copy_terms[diag] * factor;
                }
            }
        }

        vector<T, M> finite_solution;

        //there can be at max D finite solutions to equation system i.e if N >= M there is only M variables but
        //N equations so at max there can only be min(N,M) == M solutions
        //if there is N < M there is more variables than equations thus only N finite solutions (constant solution)
        //other variables can be interpreted as parameters to solution and used to create vectors
        //forming infinite solution set

        bool non_finite_solution_variables[M];
        std::fill(non_finite_solution_variables, non_finite_solution_variables + M, true);

        //finding finite solutions
        for (size_t row = 0; row < N; row++)
        {
            bool found = false;

            for (size_t column = 0; column < M; column++)
            {
                //first nonzero element in row is treated as finite solution variable
                if (!equal(copy[row][column], static_cast<T>(0)))
                {
                    finite_solution[column] = copy_terms[row];
                    non_finite_solution_variables[column] = false;
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                if (!equal(copy_terms[row], static_cast<T>(0)))
                {
                    //system is contradictory
                    return std::nullopt;
                }
            }
        }

        //variables that have no finite solution (constant solution), i.e that haven't been assigned to 
        //some constant term are treated as parameters to solution and are used to create vectors
        //forming infinite solution set
        std::vector<vector<T, M>> infinite_solution_vectors;
        for (size_t column = 0; column < M; column++)
        {
            if(non_finite_solution_variables[column])
            {
                vector<T, M> infinite_solution_vector;

                //variable is treated as parameter
                infinite_solution_vector[column] = 1;

                //creating infinite solution forming vector
                for (size_t row = 0; row < D; row++)
                {
                    if (!equal(copy[row][column], static_cast<T>(0)))
                    {
                        infinite_solution_vector[row] = static_cast<T>(-1)*copy[row][column];
                    }
                }

                infinite_solution_vectors.push_back(infinite_solution_vector);
            }
        }

        return std::make_pair(finite_solution, infinite_solution_vectors);
    };

    return solve_with_gaussian_elimination();
}

NAMESPACE_LINEAR_ALGEBRA_END