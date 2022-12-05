<?php
declare(strict_types=1);

function foo(int $var2, int $var3):string{
    write($var2);
    write($var3);
    $var2 = readi();
    $var3 = readi();
    foo($var2, $var3);
    
}
$var2 = readi();
$var3 = readi();
foo($var2, $var3);
