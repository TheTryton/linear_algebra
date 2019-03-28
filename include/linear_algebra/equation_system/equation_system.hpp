#pragma once

#include "../matrix/matrix.hpp"
#include "../vector/vector.hpp"
#include "../common_matrix_vector_operations.inl"

NAMESPACE_LINEAR_ALGEBRA_BEGIN

template<class T, size_t M>
class equation_system_solution
{
public:
    struct determinate_solution
    {
        vector<T, M>                constant_solution_vector; // terms in solution that does not depend on any parameters

        //constructors
        determinate_solution() = delete;

        determinate_solution(const determinate_solution& other) :
            constant_solution_vector(other.constant_solution_vector)
        {
        }
        determinate_solution(determinate_solution&& other) :
            constant_solution_vector(std::move(other.constant_solution_vector))
        {
        }
        determinate_solution& operator=(const determinate_solution& other)
        {
            constant_solution_vector = other.constant_solution_vector;
            return *this;
        }
        determinate_solution& operator=(determinate_solution&& other)
        {
            constant_solution_vector = std::move(other.constant_solution_vector);
            return *this;
        }

        determinate_solution(const vector<T, M>& constant_solution_vector) :
            constant_solution_vector(constant_solution_vector)
        {
        }
        determinate_solution(vector<T, M>&& constant_solution_vector) :
            constant_solution_vector(constant_solution_vector)
        {
        }
    };
    struct indeterminate_solution
    {
        vector<T, M>                constant_solution_vector; // terms in solution that does not depend on any parameters
        std::vector<vector<T, M>>   infinite_solution_vectors; // groups of terms in solution that depend on parameters (grouped by parameter)

        //constructors
        indeterminate_solution() = delete;

        indeterminate_solution(const indeterminate_solution& other) :
            constant_solution_vector(other.constant_solution_vector),
            infinite_solution_vectors(other.infinite_solution_vectors)
        {
        }
        indeterminate_solution(indeterminate_solution&& other) :
            constant_solution_vector(std::move(other.constant_solution_vector)),
            infinite_solution_vectors(std::move(other.infinite_solution_vectors))
        {
        }
        indeterminate_solution& operator=(const indeterminate_solution& other)
        {
            constant_solution_vector = other.constant_solution_vector;
            infinite_solution_vectors = other.infinite_solution_vectors;
            return *this;
        }
        indeterminate_solution& operator=(indeterminate_solution&& other)
        {
            constant_solution_vector = std::move(other.constant_solution_vector);
            infinite_solution_vectors = std::move(other.infinite_solution_vectors);
            return *this;
        }

        indeterminate_solution(const vector<T, M>& constant_solution_vector, const std::vector<vector<T, M>>& infinite_solution_vectors) :
            constant_solution_vector(constant_solution_vector),
            infinite_solution_vectors(infinite_solution_vectors)
        {
        }
        indeterminate_solution(vector<T, M>&& constant_solution_vector, std::vector<vector<T, M>>&& infinite_solution_vectors) :
            constant_solution_vector(constant_solution_vector),
            infinite_solution_vectors(infinite_solution_vectors)
        {
        }
    };
private:
    equation_system_type _type;
    using solution_type = std::optional<std::variant<determinate_solution, indeterminate_solution>>;
    solution_type _solution;

    template<equation_system_type system_type>
    using result_type = std::conditional_t<
        system_type == equation_system_type::determinate, const determinate_solution&,
        std::conditional_t<
        system_type == equation_system_type::indeterminate, const indeterminate_solution&,
        void
        >>;
public:
    equation_system_solution():
        _type(equation_system_type::contradictory)
    {
    }

    equation_system_solution(const equation_system_solution& other) :
        _type(other._type),
        _solution(other._solution)
    {
    }
    equation_system_solution(equation_system_solution&& other) :
        _type(std::move(other._type)),
        _solution(std::move(other._solution))
    {
        other._type = equation_system_type::contradictory;
    }
    equation_system_solution& operator=(const equation_system_solution& other)
    {
        _type = other._type;
        _solution = other._solution;
        return *this;
    }
    equation_system_solution& operator=(equation_system_solution&& other)
    {
        _type = std::move(other._type);
        _solution = std::move(other._solution);
        other._type = equation_system_type::contradictory;
        return *this;
    }

    equation_system_solution(const determinate_solution& determinate_solution) :
        _type(equation_system_type::determinate),
        _solution(determinate_solution)
    {
    }
    equation_system_solution(determinate_solution&& determinate_solution) :
        _type(equation_system_type::determinate),
        _solution(determinate_solution)
    {
    }
    equation_system_solution& operator=(const determinate_solution& determinate_solution)
    {
        _type = equation_system_type::determinate;
        _solution = determinate_solution;
        return *this;
    }
    equation_system_solution& operator=(determinate_solution&& determinate_solution)
    {
        _type = equation_system_type::determinate;
        _solution = determinate_solution;
        return *this;
    }

    equation_system_solution(const indeterminate_solution& indeterminate_solution) :
        _type(equation_system_type::indeterminate),
        _solution(indeterminate_solution)
    {
    }
    equation_system_solution(indeterminate_solution&& indeterminate_solution) :
        _type(equation_system_type::indeterminate),
        _solution(indeterminate_solution)
    {
    }
    equation_system_solution& operator=(const indeterminate_solution& indeterminate_solution)
    {
        _type = equation_system_type::indeterminate;
        _solution = indeterminate_solution;
        return *this;
    }
    equation_system_solution& operator=(indeterminate_solution&& indeterminate_solution)
    {
        _type = equation_system_type::indeterminate;
        _solution = indeterminate_solution;
        return *this;
    }
public:
    equation_system_type system_type() const
    {
        return _type;
    }

    determinate_solution& get_determinate_solution()
    {
        return std::get<0>(*_solution);
    }
    const determinate_solution& get_determinate_solution() const
    {
        return std::get<0>(*_solution);
    }
    indeterminate_solution& get_indeterminate_solution()
    {
        return std::get<1>(*_solution);
    }
    const indeterminate_solution& get_indeterminate_solution() const
    {
        return std::get<1>(*_solution);
    }

    template<equation_system_type system_type>
    result_type<system_type> get_result() const
    {
        if constexpr (system_type == equation_system_type::determinate)
        {
            return std::get<0>(*_solution);
        }
        else if constexpr (system_type == equation_system_type::indeterminate)
        {
            return std::get<1>(*_solution);
        }
        else
        {
            return;
        }
    }
public:
    operator bool() const
    {
        return static_cast<bool>(_solution) && (_type != equation_system_type::contradictory);
    }
};

///<summary>
/// solves linear equations system (N equations, M variables)
/// <para>*** RESULTS ***</para>
/// <para>if equation system is contradictory returns nullopt</para>
/// <para>if equation system is indeterminate returns constant shift vector and set of vectors creating infinite solution set</para>
/// <para>if equation system is determinate returns solution</para>
///</summary>
/// <param name="coefficents"> linear equation system coefficents represented in NxM matrix </param>
/// <param name="constant_terms"> linear equation system constant terms </param>
template<class T, size_t N, size_t M>
equation_system_solution<T, M> solve_equation_system(const matrix<T, N, M>& coefficents, const vector<T, N>& constant_terms)
{
    auto copy = coefficents;
    auto copy_terms = constant_terms;

    constexpr size_t D = N > M ? M : N;

    //using gaussian elimination to obtain matrix in row
    //echelon form

    size_t column_shift = 0;
    for (size_t diag = 0; diag < D; diag++)
    {
        bool no_non_zero = false;

        while (equal(copy[diag][diag + column_shift], static_cast<T>(0)))
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

#if USE_OPENMP
        if (copy.is_big_matrix)
        {
            for (size_t column = diag + column_shift; column < M; column++)
            {
                copy[diag][column] /= factor;
            }
            copy_terms[diag] /= factor;

#pragma omp parallel for
            for (int reduced_row = 0; reduced_row < static_cast<int>(diag); reduced_row++)
            {
                T factor = copy._mat[reduced_row][diag + column_shift] / copy._mat[diag][diag + column_shift];
                for (size_t column = diag + column_shift; column < M; column++)
                {
                    copy._mat[reduced_row][column] -= copy._mat[diag][column] * factor;
                }

                copy_terms[reduced_row] -= copy_terms[diag] * factor;
            }

#pragma omp parallel for
            for (int reduced_row = static_cast<int>(diag + 1); reduced_row < static_cast<int>(N); reduced_row++)
            {

                T factor = copy._mat[reduced_row][diag + column_shift] / copy._mat[diag][diag + column_shift];

                for (size_t column = diag + column_shift; column < M; column++)
                {
                    copy._mat[reduced_row][column] -= copy._mat[diag][column] * factor;
                }

                copy_terms[reduced_row] -= copy_terms[diag] * factor;
            }
        }
        else
        {
            for (size_t column = diag + column_shift; column < M; column++)
            {
                copy[diag][column] /= factor;
            }
            copy_terms[diag] /= factor;

            for (size_t reduced_row = 0; reduced_row < diag; reduced_row++)
            {
                T factor = copy._mat[reduced_row][diag + column_shift] / copy._mat[diag][diag + column_shift];
                for (size_t column = diag + column_shift; column < M; column++)
                {
                    copy._mat[reduced_row][column] -= copy._mat[diag][column] * factor;
                }

                copy_terms[reduced_row] -= copy_terms[diag] * factor;
            }

            for (size_t reduced_row = diag + 1; reduced_row < N; reduced_row++)
            {

                T factor = copy._mat[reduced_row][diag + column_shift] / copy._mat[diag][diag + column_shift];

                for (size_t column = diag + column_shift; column < M; column++)
                {
                    copy._mat[reduced_row][column] -= copy._mat[diag][column] * factor;
                }

                copy_terms[reduced_row] -= copy_terms[diag] * factor;
            }
        }
#else
        for (size_t column = diag + column_shift; column < M; column++)
        {
            copy[diag][column] /= factor;
        }
        copy_terms[diag] /= factor;

        for (size_t reduced_row = 0; reduced_row < diag; reduced_row++)
        {
            T factor = copy._mat[reduced_row][diag + column_shift] / copy._mat[diag][diag + column_shift];
            for (size_t column = diag + column_shift; column < M; column++)
            {
                copy._mat[reduced_row][column] -= copy._mat[diag][column] * factor;
            }

            copy_terms[reduced_row] -= copy_terms[diag] * factor;
        }

        for (size_t reduced_row = diag + 1; reduced_row < N; reduced_row++)
        {

            T factor = copy._mat[reduced_row][diag + column_shift] / copy._mat[diag][diag + column_shift];

            for (size_t column = diag + column_shift; column < M; column++)
            {
                copy._mat[reduced_row][column] -= copy._mat[diag][column] * factor;
            }

            copy_terms[reduced_row] -= copy_terms[diag] * factor;
        }
#endif
    }

    vector<T, M> constant_solution_vector;

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
                constant_solution_vector[column] = copy_terms[row];
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
                return equation_system_solution<T, M>();
            }
        }
    }

    //variables that have no finite solution (constant solution), i.e that haven't been assigned to 
    //some constant term are treated as parameters to solution and are used to create vectors
    //forming infinite solution set
    std::vector<vector<T, M>> infinite_solution_vectors;
    for (size_t column = 0; column < M; column++)
    {
        if (non_finite_solution_variables[column])
        {
            vector<T, M> infinite_solution_vector;

            //variable is treated as parameter
            infinite_solution_vector[column] = static_cast<T>(1);

            //creating infinite solution forming vector
            for (size_t row = 0; row < D; row++)
            {
                if (!equal(copy[row][column], static_cast<T>(0)) && row != column)
                {
                    infinite_solution_vector[row] = static_cast<T>(-1)*copy[row][column];
                }
            }

            infinite_solution_vectors.push_back(infinite_solution_vector);
        }
    }

    if (infinite_solution_vectors.empty())
    {
        //system is determinate
        return equation_system_solution<T, M>(std::move(constant_solution_vector));
    }
    else
    {
        //system is indeterminate
        return equation_system_solution<T, M>(std::move(constant_solution_vector), std::move(infinite_solution_vectors));
    }
}

///<summary>
/// solves linear equations system (N equations, M variables)
/// <para>*** RESULTS ***</para>
/// <para>if equation system is contradictory returns nullopt</para>
/// <para>if equation system is indeterminate returns constant shift vector and set of vectors creating infinite solution set</para>
/// <para>if equation system is determinate returns solution</para>
///</summary>
/// <param name="coefficents"> linear equation system coefficents represented in NxM matrix </param>
/// <param name="constant_terms"> linear equation system constant terms </param>
template<class T, size_t N, size_t M>
equation_system_solution<T, M> solve_equation_system(matrix<T, N, M>&& coefficents, vector<T, N>&& constant_terms)
{
    constexpr size_t D = N > M ? M : N;

    //using gaussian elimination to obtain matrix in row
    //echelon form

    size_t column_shift = 0;
    for (size_t diag = 0; diag < D; diag++)
    {
        bool no_non_zero = false;

        while (equal(coefficents[diag][diag + column_shift], static_cast<T>(0)))
        {
            size_t non_zero_row = diag;

            for (size_t f_row = diag + 1; f_row < N; f_row++)
            {
                if (!equal(coefficents[f_row][diag + column_shift], static_cast<T>(0)))
                {
                    non_zero_row = f_row;
                    break;
                }
            }

            if (non_zero_row != diag)
            {
                coefficents.swap_rows(coefficents, diag, non_zero_row, diag + column_shift);

                std::swap(constant_terms[diag], constant_terms[non_zero_row]);
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

        T factor = coefficents[diag][diag + column_shift];

#if USE_OPENMP
        if (coefficents.is_big_matrix)
        {
            for (size_t column = diag + column_shift; column < M; column++)
            {
                coefficents[diag][column] /= factor;
            }
            constant_terms[diag] /= factor;

#pragma omp parallel for
            for (int reduced_row = 0; reduced_row < static_cast<int>(diag); reduced_row++)
            {
                T factor = coefficents._mat[reduced_row][diag + column_shift] / coefficents._mat[diag][diag + column_shift];
                for (size_t column = diag + column_shift; column < M; column++)
                {
                    coefficents._mat[reduced_row][column] -= coefficents._mat[diag][column] * factor;
                }

                constant_terms[reduced_row] -= constant_terms[diag] * factor;
            }

#pragma omp parallel for
            for (int reduced_row = static_cast<int>(diag + 1); reduced_row < static_cast<int>(N); reduced_row++)
            {

                T factor = coefficents._mat[reduced_row][diag + column_shift] / coefficents._mat[diag][diag + column_shift];

                for (size_t column = diag + column_shift; column < M; column++)
                {
                    coefficents._mat[reduced_row][column] -= coefficents._mat[diag][column] * factor;
                }

                constant_terms[reduced_row] -= constant_terms[diag] * factor;
            }
        }
        else
        {
            for (size_t column = diag + column_shift; column < M; column++)
            {
                coefficents[diag][column] /= factor;
            }
            constant_terms[diag] /= factor;

            for (size_t reduced_row = 0; reduced_row < diag; reduced_row++)
            {
                T factor = coefficents._mat[reduced_row][diag + column_shift] / coefficents._mat[diag][diag + column_shift];
                for (size_t column = diag + column_shift; column < M; column++)
                {
                    coefficents._mat[reduced_row][column] -= coefficents._mat[diag][column] * factor;
                }

                constant_terms[reduced_row] -= constant_terms[diag] * factor;
            }

            for (size_t reduced_row = diag + 1; reduced_row < N; reduced_row++)
            {

                T factor = coefficents._mat[reduced_row][diag + column_shift] / coefficents._mat[diag][diag + column_shift];

                for (size_t column = diag + column_shift; column < M; column++)
                {
                    coefficents._mat[reduced_row][column] -= coefficents._mat[diag][column] * factor;
                }

                constant_terms[reduced_row] -= constant_terms[diag] * factor;
            }
        }
#else
        for (size_t column = diag + column_shift; column < M; column++)
        {
            coefficents[diag][column] /= factor;
        }
        constant_terms[diag] /= factor;

        for (size_t reduced_row = 0; reduced_row < diag; reduced_row++)
        {
            T factor = coefficents._mat[reduced_row][diag + column_shift] / coefficents._mat[diag][diag + column_shift];
            for (size_t column = diag + column_shift; column < M; column++)
            {
                coefficents._mat[reduced_row][column] -= coefficents._mat[diag][column] * factor;
            }

            constant_terms[reduced_row] -= constant_terms[diag] * factor;
        }

        for (size_t reduced_row = diag + 1; reduced_row < N; reduced_row++)
        {

            T factor = coefficents._mat[reduced_row][diag + column_shift] / coefficents._mat[diag][diag + column_shift];

            for (size_t column = diag + column_shift; column < M; column++)
            {
                coefficents._mat[reduced_row][column] -= coefficents._mat[diag][column] * factor;
            }

            constant_terms[reduced_row] -= constant_terms[diag] * factor;
        }
#endif
    }

    vector<T, M> constant_solution_vector;

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
            if (!equal(coefficents[row][column], static_cast<T>(0)))
            {
                constant_solution_vector[column] = constant_terms[row];
                non_finite_solution_variables[column] = false;
                found = true;
                break;
            }
        }

        if (!found)
        {
            if (!equal(constant_terms[row], static_cast<T>(0)))
            {
                //system is contradictory
                return equation_system_solution<T, M>();
            }
        }
    }

    //variables that have no finite solution (constant solution), i.e that haven't been assigned to 
    //some constant term are treated as parameters to solution and are used to create vectors
    //forming infinite solution set
    std::vector<vector<T, M>> infinite_solution_vectors;
    for (size_t column = 0; column < M; column++)
    {
        if (non_finite_solution_variables[column])
        {
            vector<T, M> infinite_solution_vector;

            //variable is treated as parameter
            infinite_solution_vector[column] = static_cast<T>(1);

            //creating infinite solution forming vector
            for (size_t row = 0; row < D; row++)
            {
                if (!equal(coefficents[row][column], static_cast<T>(0)) && row != column)
                {
                    infinite_solution_vector[row] = static_cast<T>(-1)*coefficents[row][column];
                }
            }

            infinite_solution_vectors.push_back(infinite_solution_vector);
        }
    }

    if (infinite_solution_vectors.empty())
    {
        //system is determinate
        return equation_system_solution<T, M>(equation_system_solution<T, M>::determinate_solution(std::move(constant_solution_vector)));
    }
    else
    {
        //system is indeterminate
        return equation_system_solution<T, M>(equation_system_solution<T, M>::indeterminate_solution(std::move(constant_solution_vector), std::move(infinite_solution_vectors)));
    }
}

NAMESPACE_LINEAR_ALGEBRA_END