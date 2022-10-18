<?php
declare(strict_types=1);
function concat(string $x, string $y): string {
    $x = $x . $y;
    return $x . " " . $y; }
$a = "ahoj ";
$ret = concat($a, "svete"); write($ret, $a);
?>