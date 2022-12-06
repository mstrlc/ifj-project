<?php
declare(strict_types=1);
$var1 = 0;
$str = "EZ FULL BODY    ";
$strRes = "";
while($var1 < 10 ){
    $strRes = $strRes . $str;
    $var1 = $var1 + 1;
    write($var1, " ");
}
write($strRes);   