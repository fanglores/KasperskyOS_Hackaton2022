#include <vector>
#include <iostream>

using namespace std;

vector <vector<double>> inversion(vector<vector<double>> A)
{
    int N = A.size();
    double temp;

    vector<vector<double>> E;
    E.resize(N);
    for (int i = 0; i < N; i++) {
        E[i].resize(N);
    }

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
        {
            E[i][j] = 0.0;

            if (i == j)
                E[i][j] = 1.0;
        }

    for (int k = 0; k < N; k++)
    {
        temp = A[k][k];

        for (int j = 0; j < N; j++)
        {
            A[k][j] /= temp;
            E[k][j] /= temp;
        }

        for (int i = k + 1; i < N; i++)
        {
            temp = A[i][k];

            for (int j = 0; j < N; j++)
            {
                A[i][j] -= A[k][j] * temp;
                E[i][j] -= E[k][j] * temp;
            }
        }
    }

    for (int k = N - 1; k > 0; k--)
    {
        for (int i = k - 1; i >= 0; i--)
        {
            temp = A[i][k];

            for (int j = 0; j < N; j++)
            {
                A[i][j] -= A[k][j] * temp;
                E[i][j] -= E[k][j] * temp;
            }
        }
    }

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            A[i][j] = E[i][j];

    return A;
}

vector <vector <double>> make_matrix_trans(vector <double> x_floor, vector <double> y_floor,
	vector <double> x_screen, vector <double> y_screen)
{
    
	vector <vector<double>> matrix_trans(3, vector<double>(3));
	double delta0 = (x_screen[1] - x_screen[2]) * (y_screen[3] - y_screen[2]) -
		(y_screen[1] - y_screen[2]) * (x_screen[3] - x_screen[2]);
    cout << delta0 << endl;
	double delta1 = (x_screen[0] - x_screen[1] + x_screen[2] - x_screen[3]) * (y_screen[3] - y_screen[2]) -
		(y_screen[0] - y_screen[1] + y_screen[2] - y_screen[3]) * (x_screen[3] - x_screen[2]);
    cout << delta1 << endl;
	double delta2 = (x_screen[1] - x_screen[2]) * (y_screen[0] - y_screen[1] + y_screen[2] - y_screen[3]) -
		(y_screen[1] - y_screen[2]) * (x_screen[0] - x_screen[1] + x_screen[2] - x_screen[3]);
    cout << delta2 << endl;
	vector <vector<double>> matrix_1_screen(3, vector<double>(3));
	matrix_1_screen[0][2] = delta1 / delta0;
	matrix_1_screen[1][2] = delta2 / delta0;
	matrix_1_screen[0][0] = x_screen[1] - x_screen[0] + matrix_1_screen[0][2] * x_screen[1];
	matrix_1_screen[0][1] = y_screen[1] - y_screen[0] + matrix_1_screen[0][2] * y_screen[1];
	matrix_1_screen[1][0] = x_screen[3] - x_screen[0] + matrix_1_screen[1][2] * x_screen[3];
	matrix_1_screen[1][1] = y_screen[3] - y_screen[0] + matrix_1_screen[1][2] * y_screen[3];
	matrix_1_screen[2][0] = x_screen[0];
	matrix_1_screen[2][1] = y_screen[0];
    matrix_1_screen[2][2] = 1.0;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout << matrix_1_screen[i][j];
        }
        cout << endl;
    }

    vector<vector<double>> matrix_screen_1(3, vector<double>(3));
    matrix_screen_1 = inversion(matrix_1_screen);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout << matrix_screen_1[i][j];
        }
        cout << endl;
    }

    vector<vector<double>> matrix_1_floor(3, vector<double>(3));

    double deltaf0 = (x_floor[1] - x_floor[2]) * (y_floor[3] - y_floor[2]) -
        (y_floor[1] - y_floor[2]) * (x_floor[3] - x_floor[2]);
    double deltaf1 = (x_floor[0] - x_floor[1] + x_floor[2] - x_floor[3]) * (y_floor[3] - y_floor[2]) -
        (y_floor[0] - y_floor[1] + y_floor[2] - y_floor[3]) * (x_floor[3] - x_floor[2]);
    double deltaf2 = (x_floor[1] - x_floor[2]) * (y_floor[0] - y_floor[1] + y_floor[2] - y_floor[3]) -
        (y_floor[1] - y_floor[2]) * (x_floor[0] - x_floor[1] + x_floor[2] - x_floor[3]);

    matrix_1_floor[0][2] = deltaf1 / deltaf0;
    matrix_1_floor[1][2] = deltaf2 / deltaf0;
    matrix_1_floor[0][0] = x_floor[1] - x_floor[0] + matrix_1_floor[0][2] * x_floor[1];
    matrix_1_floor[0][1] = y_floor[1] - y_floor[0] + matrix_1_floor[0][2] * y_floor[1];
    matrix_1_floor[1][0] = x_floor[3] - x_floor[0] + matrix_1_floor[1][2] * x_floor[3];
    matrix_1_floor[1][1] = y_floor[3] - y_floor[0] + matrix_1_floor[1][2] * y_floor[3];
    matrix_1_floor[2][0] = x_floor[0];
    matrix_1_floor[2][1] = y_floor[0];
    matrix_1_floor[2][2] = 1.0;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout << matrix_1_floor[i][j];
        }
        cout << endl;
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            matrix_trans[i][j] = 0;
            for (int k = 0; k < 3; k++)
                matrix_trans[i][j] += matrix_screen_1[i][k] * matrix_1_floor[k][j];
        }
    }

    return matrix_trans;
}

vector <double> new_coordinates(vector <double> old_coordinates, vector <vector<double>> matrix_trans)
{
    vector <double> new_coordinates(2);
    new_coordinates[0] = (matrix_trans[0][0] * old_coordinates[0] + matrix_trans[1][0] * old_coordinates[1] +
        matrix_trans[2][0]) / (matrix_trans[0][2] * old_coordinates[0] + matrix_trans[1][2] * old_coordinates[1] + 1);
    new_coordinates[1] = (matrix_trans[0][1] * old_coordinates[0] + matrix_trans[1][1] * old_coordinates[1] +
        matrix_trans[2][1]) / (matrix_trans[0][2] * old_coordinates[0] + matrix_trans[1][2] * old_coordinates[1] + 1);

    return new_coordinates;
}


int main()
{
    vector<double> x_floor = {0, 1, 1, 0};
    vector<double> y_floor = {0, 0, 1, 1};
    vector<double> x_screen = { 0, 500, 600, 200 };
    vector<double> y_screen = { 0, 100, 400, 500 };

    vector<vector<double>> matrix(3, vector<double>(3));
    matrix = make_matrix_trans(x_floor, y_floor, x_screen, y_screen);
    for (int i=0; i < 3; i++)
    {
        for (int j=0; j < 3; j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }

    vector<double> o_c(2);
    o_c = { 383.33, 255.556 };
    vector<double> n_c(2);
    n_c = new_coordinates(o_c, matrix);
    cout << endl;
    cout << n_c[0] << " " <<  n_c[1] << endl;
}
