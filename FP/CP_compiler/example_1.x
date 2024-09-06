# Факториал 5 #
let_rec fact 
    Lam x   
        if {$x <= 1}
        	{1}
	else   
        	{$x * $fact [{ $x - 1 }]} 
$fact [5]