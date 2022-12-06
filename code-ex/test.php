<?php
declare(strict_types=1);
$var1 = 0;
foo($var1);
function foo(int $var): void{
    write($var);
    $var = $var + 1;
    foo($var);  
}
