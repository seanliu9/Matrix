#include <stdexcept>
#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

class Matrix
{
private:
    int m; // number of rows
    int n; // number of columns
    double **vals; // 2-D array of values in the matrix

public:
    // constructors

    // default constructor
    Matrix()
    {
        this->m = 0;
        this->n = 0;
        this->vals = nullptr;
    }

    Matrix(const int num_rows, const int num_cols)
    {
        this->m = num_rows;
        this->n = num_cols;

        // Allocate memory for vals.
        this->vals = new double*[this->get_m()];
        for (int i = 0; i < this->get_m(); i++)
        {
            this->vals[i] = new double[this->get_n()];
        }
    }

    Matrix(const std::vector<std::vector<double>>& values)
    {
        this->m = values.size();
        this->n = values[0].size();

        // Allocate memory for vals.
        this->vals = new double*[this->get_m()];
        for (int i = 0; i < this->get_m(); i++)
        {
            this->vals[i] = new double[this->get_n()];
            for (int j = 0; j < this->get_n(); j++)
            {
                this->vals[i][j] = values[i][j];
            }
        }
    }

    // copy constructor
    Matrix(const Matrix& mat)
    {
        this->vals = nullptr;
        this->copy_from(mat);
    }

    // move constructor
    Matrix(Matrix&& rhs)
    {
        this->move_from(std::move(rhs));
    }

    // destructor
    ~Matrix()
    {
        if (this->get_vals() != nullptr)
        {
            // Delete each row in vals.
            for (int i = 0; i < this->get_m(); i++)
            {
                delete[] vals[i];
            }
            // Delete the array of pointers (vals).
            delete[] vals;
        }
    }

    // set and get methods

    void set_m(const int num_rows)
    {
        this->m = num_rows;
    }

    void set_n(const int num_cols)
    {
        this->n = num_cols;
    }

    void set_vals(double** values)
    {
        this->vals = values;
    }

    int get_m() const
    {
        return this->m;
    }

    int get_n() const
    {
        return this->n;
    }

    double** get_vals() const
    {
        return this->vals;
    }

    // Set a value at a particular cell.
    void set_val(const int i, const int j, const int val)
    {
        this->vals[i][j] = val;
    }

    // other methods and operators

    Matrix operator + (const Matrix& op) const
    {
        if (this->get_m() != op.get_m() || this->get_n() != op.get_n())
        {
            throw runtime_error("Matrices must have the same dimensions.");
        }
        Matrix result(this->get_m(), this->get_n());
        for (int i = 0; i < this->get_m(); i++)
        {
            for (int j = 0; j < this->get_n(); j++)
            {
                result.set_val(i, j, this->vals[i][j] + op.get_vals()[i][j]);
            }
        }
        return result;
    }

    Matrix operator * (const Matrix& op) const
    {
        if (this->get_n() != op.get_m())
        {
            throw runtime_error("Matrix 1's number of columns must equal Matrix 2's number of rows.");
        }
        Matrix result(this->get_m(), op.get_n());
        int x = 0;
        for (int i = 0; i < this->get_m(); i++)
        {
            for (int j = 0; j < op.get_n(); j++)
            {
                x = 0;
                for (int k = 0; k < this->get_n(); k++)
                {
                    x += this->vals[i][k] * op.get_vals()[k][j];
                }
                result.set_val(i, j, x);
            }
        }
        return result;
    }

    void delete_vals()
    {
        for (int i = 0; i < this->get_m(); i++)
        {
            delete[] this->vals[i];
        }
        delete[] this->vals;
    }

    void copy_from (const Matrix& mat)
    {
        if (this != &mat)
        {
            // Delete the old vals.
            if (this->get_vals() != nullptr)
            {   
                this->delete_vals();
            }

            this->m = mat.get_m();
            this->n = mat.get_n();
            // Allocate memory for vals.
            this->vals = new double*[this->get_m()];
            for (int i = 0; i < this->get_m(); i++)
            {
                this->vals[i] = new double[this->get_n()];
                for (int j = 0; j < this->get_n(); j++)
                {
                    this->set_val(i, j, mat.get_vals()[i][j]);
                }
            }
        }
    }

    void move_from(Matrix&& rhs)
    {
        // Delete the old vals.
        if (this->get_vals() != nullptr)
        {   
            this->delete_vals();
        }

        this->set_m(rhs.get_m());
        this->set_n(rhs.get_n());
        this->set_vals(rhs.get_vals());

        rhs.set_vals(nullptr);
    }

    const Matrix& operator = (const Matrix& op)
    {
        if (this != &op)
        {
            this->copy_from(op);
        }
        return *this;
    }

    const Matrix& operator = (Matrix&& rhs) noexcept
    {
        this->move_from(std::move(rhs));
        return *this;
    }

    friend ostream& operator << (ostream& os, const Matrix& op)
    {
        for (int i = 0; i < op.get_m(); i++) 
        {
            os << "[ ";
            for (int j = 0; j < op.get_n(); j++) 
            {
                os << std::setw(4) << op.get_vals()[i][j] << " ";
            }
            os << " ]" << std::endl;
        }
        return os;
    }
};

// Matrix operator + (const Matrix& op1, const Matrix& op2)
// {
//     if (op1.get_m() != op2.get_m() || op1.get_n() != op2.get_n())
//     {
//         throw runtime_error("Matrices must have the same dimensions.");
//     }
//     Matrix result(op1.get_m(), op1.get_n());
//     for (int i = 0; i < op1.get_m(); i++)
//     {
//         for (int j = 0; j < op1.get_n(); j++)
//         {
//             result.set_val(i, j, op1.get_vals()[i][j] + op2.get_vals()[i][j]);
//         }
//     }
//     return result;
// }

// Matrix operator * (const Matrix& op1, const Matrix& op2)
// {
//     if (op1.get_n() != op2.get_m())
//     {
//         throw runtime_error("Matrix 1's number of columns must equal Matrix 2's number of rows.");
//     }
//     Matrix result(op1.get_m(), op2.get_n());
//     int x = 0;
//     for (int i = 0; i < op1.get_m(); i++)
//     {
//         for (int j = 0; j < op2.get_n(); j++)
//         {
//             x = 0;
//             for (int k = 0; k < op1.get_n(); k++)
//             {
//                 x += op1.get_vals()[i][k] * op2.get_vals()[k][j];
//             }
//             result.set_val(i, j, x);
//         }
//     }
//     return result;
// }

int main()
{
    cout << "Testing copy constructor: " << endl;
    Matrix m1({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15}});
    Matrix m2(m1);
    Matrix m3;
    Matrix m4;
    m4 = m3 = m2;
    cout << "m1: " << endl << m1 << endl;
    cout << "m2: " << endl << m2 << endl;
    cout << "m3: " << endl << m3 << endl;
    cout << "m4: " << endl << m4 << endl;

    cout << "Testing matrix addition: " << endl;
    Matrix a({{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}});
    cout << "a: " << endl << a << endl;
    Matrix b ({{2, 2, 2, 2}, {2, 2, 2, 2}, {2, 2, 2, 2}});
    cout << "b: " << endl << b << endl;
    Matrix c = a + b;
    cout << "c = a + b: " << endl << c << endl;
    Matrix d = b + c;
    cout << "d = b + c:" << endl << d << endl;

    cout << "Testing matrix multiplication: " << endl;
    Matrix e({{2, 1, 3}, {1, 4, 7}});
    Matrix f({{1, 1, 2, 6}, {3, 1, 2, 3}, {2, 4, 5, 1}});
    Matrix g = e * f;
    cout << "g = e * f: " << endl << g << endl;

}