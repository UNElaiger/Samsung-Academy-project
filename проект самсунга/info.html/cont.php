<?php
if (isset($_POST['number']) && isset($_POST['numbers']) && isset($_POST['type']) && isset($_POST['kol']) && isset($_POST['date']) && isset($_POST['time'])){

// Переменные с формы
$number = $_POST['number'];
$numbers = $_POST['numbers'];
$type = $_POST['type'];
$kol = $_POST['kol'];
$date = $_POST['date'];
$time = $_POST['time'];

// Параметры для подключения
$db_host = "p533226.mysql.ihc.ru";
$db_user = "p533226_1"; // Логин БД
$db_password = "pcF8m5W7Xs"; // Пароль БД
$db_base = 'p533226_1'; // Имя БД
$db_table = "bd1"; // Имя Таблицы БД

// Подключение к базе данных
$mysqli = new mysqli($db_host,$db_user,$db_password,$db_base);
$mysqli->set_charset('Utf8');
mb_internal_encoding("UTF-8");
// Если есть ошибка соединения, выводим её и убиваем подключение
if ($mysqli->connect_error) {
    die('Ошибка : ('. $mysqli->connect_errno .') '. $mysqli->connect_error);
}

$result = $mysqli->query("INSERT INTO ".$db_table." (number,numbers,type,kol,date,time) VALUES ('$number','$numbers','$type','$kol','$date','$time')");

if ($result == true){

    $result = $mysqli->query("SELECT * FROM $db_table WHERE number = '$number'");
    $result = $result->fetch_assoc();
     if($result==true){


    require_once("services.html");
}
    //echo "Информация занесена в базу данных";
}else{
    echo "Информация не занесена в базу данных";
}
}
?>
