# Student Details:
# Student Name: Muath Abu Jamal


import numpy as np
import matplotlib.pyplot as plt
from sympy import symbols, lambdify

# This method calculates the roots according to newton's method
def newton(f, x_n, x_minus_one, epsilon):
    start_x_n = x_n
    # making derivative to the function
    diff_f = f.diff(var_x)

    for M in range(101):
        # checking if we are too close to the root
        # if true we will break the root to print the root
        if (abs(x_minus_one - x_n) < epsilon) or (abs(f.subs(var_x, x_n)) < epsilon):
            break

        # substitute the x_n in the derivative
        diff_f_sub = diff_f.subs(var_x, x_n)

        # checking if the divider is equals to zero
        # if true print a message and return None
        if diff_f_sub == 0:
            print("Newton's Method Failed => Division By Zero")
            return None

        # swapping x_n to x_minus_one and calculating the next x_n and
        temp = x_n
        x_n = float(x_n - (f.subs(var_x, x_n) / diff_f_sub))
        x_minus_one = temp

    # checking if the loop finished without finding the root or the root is spurious root
    if M == 100 or (abs(x_n - start_x_n) > 100 and float(diff_f.subs(var_x, x_n)) < epsilon):
        print("Newton's Method Answer Is => No Root Found")

    # printing the founded root in a formatted way
    else:
        print("Newton's Method Answer Is: {}   (Loops = {})".format(format(x_n, ".20f"), M + 1))
        return x_n

# This method calculates the roots according to the metars method
def metars(f, x_n, x_minus_one, epsilon):
    start_x_n = x_n

    for M in range(101):

        # checking if we are too close to the root
        # if true we will break the root to print the root
        if (abs(x_minus_one - x_n) < epsilon) or (abs(f.subs(var_x, x_n)) < epsilon):
            break

        # checking if the divider is equals to zero
        # if true print a message and return None
        if abs(f.subs(var_x, x_minus_one) - f.subs(var_x, x_n)) == 0 or abs(x_minus_one - x_n) == 0:
            print("Metar's Method Failed => Division By Zero")
            return None

        # swapping x_n to x_minus_one and calculating the next x_n and
        temp = x_n
        x_n = float(x_n - (f.subs(var_x, x_n) / ((f.subs(var_x, x_minus_one) - f.subs(var_x, x_n)) / (x_minus_one - x_n))))
        x_minus_one = temp

    # checking if the loop finished without finding the root or the root is spurious root
    if M == 100 or (abs(x_n - start_x_n) > 100 and (
            (f.subs(var_x, x_minus_one) - f.subs(var_x, x_n)) / (x_minus_one - x_n)) < epsilon):
        print("Metar's Method Answer Is => No Root Found")

    # printing the founded root in a formatted way
    else:
        print("Metar's Method Answer Is: {}   (Loops = {})".format(format(x_n, ".20f"), M + 1))
        return x_n


# This method is for printing the graph using the matplot library
def graph_print(f, root):
    x = np.arange(root - 10, root + 10, 0.01)
    y = lambdify(var_x, f, "numpy")

    plt.plot(x, y(x))
    plt.grid(True)
    plt.axhline(0, color='k')
    plt.axvline(0, color='k')
    axes = plt.gca()
    axes.set_xlim([int(root - 10), int(root + 10)])
    axes.set_ylim([int(- 5), int(10)])
    plt.scatter(root, 0, s=120, edgecolors='red')
    plt.xlabel("(X) axes")
    plt.ylabel("(Y) axes")
    plt.title("f(x) = x^5 - 10")
    plt.show()


# declaring a symbol 'x' and epsilon
var_x = symbols('x')
epsilon = 10 ** (-9)

f = var_x**5 - 10

# declaring and initializing the x_n and x_minus_one variables
x_n = 2
x_minus_one = x_n + (100 * epsilon)

# printing the results of the functions
print("\nCode output:")
result = newton(f, x_n, x_minus_one, epsilon)
metars(f, x_n, x_minus_one, epsilon)

# printing the graph if there is a root found
if result is not None:
    graph_print(f, result)
