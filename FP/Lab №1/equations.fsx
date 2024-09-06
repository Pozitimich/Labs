//Спирин Тимофей, М8О-213Б-22
//Вариант 18

//Уравнения 18,19,20
//---------------------------------------------------------------------------------

let eps = 1e-10

let dichotomy f a b eps =
    let rec rec_calc a b =
        let c = (a + b) / 2.0
        if abs (b - a) <= eps then
            c
        else if f a * f c <= 0.0 then
            rec_calc a c
        else
            rec_calc c b
    rec_calc a b

let rec iterations phi x eps =
    let x_new = phi x
    if abs (x_new - x) <= eps then x_new
    else iterations phi x_new eps

let newthon f f' x eps =
    let phi f f' x = x - (f x) / (f' x)
    iterations(phi f f') x eps

// Уравнение 18: x + sqrt(x) + sqrt3(x) - 2.5 = 0, интервал [0.4,1]
let f1 x = x + sqrt x + x ** (1./3.) - 2.5
let f1' x = 1. + 1. / (2. * sqrt x)  + 1. / (3. * x ** (2./3.))

// Уравнение 19: x - 1/(3 + sin (3.6*x)) = 0, интервал [0,0.85]
let f2 x = x - 1. / (3. + sin (3.6 * x))
let f2' x = 1. + 18. * cos (3.6 * x) / (5. * (sin (3.6 * x) ** 2. + 6. * sin (3.6 * x) + 9.))

// Уравнение 20: 0.1 * x^2 - x * ln x = 0, интервал [1,2]
let f3 x = 0.1 * x * x - x *  log x
let f3' x = 0.2 * x - log x - 1.

let phi1 x = x - f1 x / f1' x
let phi2 x = x - f2 x / f2' x
let phi3 x = x - f3 x / f3' x

printfn "%10.5f  %10.5f  %10.5f" (dichotomy f1 0.4 1.0 eps) (iterations phi1 0.5 eps) (newthon f1 f1' 0.5 eps)
printfn "%10.5f  %10.5f  %10.5f" (dichotomy f2 0.0 0.85 eps) (iterations phi2 0.4 eps) (newthon f2 f2' 0.4 eps)
printfn "%10.5f  %10.5f  %10.5f" (dichotomy f3 1.0 2.0 eps) (iterations phi3 1.1 eps) (newthon f3 f3' 1.3 eps)