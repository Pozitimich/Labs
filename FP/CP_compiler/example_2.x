# Одно из решений квадратного уравнения #
# ax^2 + bx + c = 0 #

let f Lam a Lam b Lam c Lam ans
	if {$a = 0} 
		if {$b = 0} void
		else {0 - $c / $b}
	else
	let d {$b * $b - 4 * $a * $c}
	if {$ans = 0} {(0 - $b - sqrt $d) / (2 * $a)}
	else
	if {$ans = 1} {(0 - $b + sqrt $d) / (2 * $a)}
	else void
	 
$f [1 1 -2 0] # -2, $f [1 1 -2 1] выдаст 1 #