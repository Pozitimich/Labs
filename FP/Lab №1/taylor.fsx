//Спирин Тимофей, М8О-213Б-22
//Вариант 18

//Ряд 18.
//---------------------------------------------------------------------------------
let eps = 1e-10

let f x = ((1. + x * x) * atan(x) - x) / 2. 

let a = 0.1
let b = 0.6
let n = 10

let rec my_pow bas power =
    if power = 0 then 1.
    elif power > 0 then bas * my_pow bas (power - 1)
    else 1. / (my_pow bas -power)

let t_term n x =
    my_pow (-1.) (n + 1) * my_pow x (2 * n + 1) / float (4 * n * n - 1)

let t_multiplier n x = 
    let n1 = float (n - 1)
    -x * x * (4. * n1 * n1 - 1.) / (4. * n1 * n1 + 8. * n1 + 3.)

let taylor_naive x eps =
    let rec rec_calc n prev_sum =
        let term = t_term n x
        let cur_sum = prev_sum + term
        if (abs(term) <= eps) then
            (cur_sum, n)
        else
            rec_calc (n + 1) cur_sum
    rec_calc 1 0.
let taylor x eps =
    let rec rec_calc n prev_term prev_sum =
        let term = if (n = 1) then (t_term 1 x) else (prev_term * t_multiplier n x)
        let cur_sum = prev_sum + term
        if (abs(term) <= eps) then
            (cur_sum, n)
        else
            rec_calc (n + 1) term cur_sum
    rec_calc 1 0. 0.

let main = 
    printfn "x\t|\tFunction Value\t|\tDumb Taylor\t|\tterms\t|\tSmart Taylor\t|\tterms"
    for i=0 to n do
        let x = a+(float i)/(float n)*(b-a)
        let (smart_val, smart_terms) = taylor x eps
        let (dumb_val, dumb_terms) = taylor_naive x eps 
        printfn "%5.2f\t|\t%10.6f\t|\t%10.6f\t|\t%d\t|\t%10.6f\t|\t%d" x (f x) (dumb_val) (dumb_terms) (smart_val) (smart_terms) 

main