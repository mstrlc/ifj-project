<?php
declare(strict_types=1);
while($dsa){
    $var = reads();
    $var2 = readi();
    $var3 = readi();
    write($var,$var2,$var3);
    $res = substring($var, $var2 ,$var3);
    write($res);
}


