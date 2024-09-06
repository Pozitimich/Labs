# Факториал 5, реализация через хвостовую рекурсию #
let fact Lam x
	let_rec fact' Lam x Lam accum
        	if {$x <= 1}
        		$accum
		else   
        		{$fact'[{$x - 1} {$accum * $x}]}
	$fact'[$x 1]
$fact[5]
		