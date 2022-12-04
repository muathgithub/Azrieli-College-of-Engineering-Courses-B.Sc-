# Student Details:
# Student Name: Muath Abu Jamal


import math
import numpy as np
import matplotlib.pyplot as plt
from sympy import symbols, lambdify


# This method generates x and y values in the range [-6, 6]
# by dividing the range into equal parts according to the given number of splines
# the first x value are ( -6 ) and the last one is ( 6 ) which are the edges of the range
# at the end it returns the generated values of x and y
def generate_xy(splines_num, _from, _to):
    step = (abs(_from) + abs(_to)) / splines_num
    xs = np.array([_from])
    ys = np.array([])

    for i in range(0, splines_num + 1):
        if i != 0:
            if i == splines_num:
                xs = np.append(xs, _to)
            else:
                xs = np.append(xs, (_from + step * i))

        sin_plus_cos = math.sin(xs[i]) + math.cos(xs[i])
        third_sqrt = abs(sin_plus_cos) ** (1 / 3)
        if sin_plus_cos < 0:
            third_sqrt *= -1

        ys = np.append(ys, third_sqrt)

    return xs, ys


# This method calculates the values of zi which is the second derivative in each x value according to the suitable
# also it calculates hi which needed to calculate zi, ci and di
# at the end it returns hi and zi
def calc_zi(splines_num, xs, ys):
    hi = np.array([])
    bi = np.array([])
    ui = np.array([0])
    vi = np.array([0])
    zi = np.zeros(splines_num + 1)

    for i in range(0, splines_num):
        hi = np.append(hi, (xs[i + 1] - xs[i]))
        bi = np.append(bi, ((6 / hi[i]) * (ys[i + 1] - ys[i])))

    ui = np.append(ui, (2 * (hi[0] + hi[1])))
    vi = np.append(vi, (bi[1] - bi[0]))

    for k in range(2, splines_num):
        ui = np.append(ui, (2 * (hi[k - 1] + hi[k]) - (((hi[k - 1]) ** 2) / (ui[k - 1]))))
        vi = np.append(vi, (bi[k] - bi[k - 1] - ((((hi[k - 1]) ** 2) / (ui[k - 1])) * vi[k - 1])))

    for j in range(splines_num - 1, 0, -1):
        zi[j] = ((vi[j] - (hi[j] * zi[j + 1])) / ui[j])

    return hi, zi


# This method calculates the values of ci and di
# according to the algorithm/way that we learnt in the class
# ad it returns them
def calc_ci_di(splines_num, ys, hi, zi):
    ci = np.array([])
    di = np.array([])

    for i in range(0, splines_num):
        ci = np.append(ci, ((ys[i + 1] / hi[i]) - ((zi[i + 1] * hi[i]) / 6)))
        di = np.append(di, ((ys[i] / hi[i]) - ((zi[i] * hi[i]) / 6)))

    return ci, di


# This method generates the splines functions and puts them in a one list
# at the id it returns that list so we can use to print the graph
def generate_splines_functions(splines_num, xs, hi, zi, ci, di):
    splines_list = []

    for i in range(0, splines_num):
        splines_list.append(((zi[i] / (6 * hi[i])) * ((xs[i + 1] - var_x) ** 3)) + (
                (zi[i + 1] / (6 * hi[i])) * ((var_x - xs[i]) ** 3)) + (ci[i] * (var_x - xs[i])) + (
                                    di[i] * (xs[i + 1] - var_x)))

    return splines_list


# This method prints the graph of the function according to the given splines list
# it goes through the splines and for each one it prints it's graph in it's range
# (according to the division of the range [-6, 6] to equal parts between the splines)
def graph_print(splines_num, _from, _to, splines_list):
    step = (abs(_from) + abs(_to)) / splines_num

    for i in range(0, splines_num):
        x = np.arange(_from + step * i, _from + step * (i + 1), 0.01)
        y = lambdify(var_x, splines_list[i], "numpy")
        plt.plot(x, y(x))
        plt.title("{:d} - Splines".format(splines_num))

    plt.grid(True)
    plt.axhline(0, color='k')
    plt.axvline(0, color='k')
    axes = plt.gca()
    axes.set_xlim([int(_from - 1), int(_to + 1)])
    axes.set_ylim([int(- 3), int(3)])
    plt.xlabel("(X) axes")
    plt.ylabel("(Y) axes")
    plt.show()


# declaring a symbol 'x'
var_x = symbols('x')

# declaring and initializing the needed spline numbers and the range [-6, 6]
splines_nums_list = [2, 4, 6, 12]
main_from = -6
main_to = 6

# for each splines num we generate the splines after making the needed calculations then we print it's graph
for i in range(0, len(splines_nums_list)):
    xs, ys = generate_xy(splines_nums_list[i], main_from, main_to)

    hi, zi = calc_zi(splines_nums_list[i], xs, ys)

    ci, di = calc_ci_di(splines_nums_list[i], ys, hi, zi)

    splines_list = generate_splines_functions(splines_nums_list[i], xs, hi, zi, ci, di)

    graph_print(splines_nums_list[i], main_from, main_to, splines_list)
