<?php

    //if (isset($_POST['r'])) $string_c = htmlentities($_POST['r']);


    //if (true) {

        // $c = (string)$string_c;

        // //Формируем JSON ответ от сервера
        // $arr = array('r' => $c);


        //$body=file_get_contents('php://input');
        //echo json_encode($body);
   //}
    //else {
      //  echo "error";
  //  }
 if(isset($_POST['number']))

  {

    // Переменные с формы

    $text = $_POST['number'];

    // Параметры для подключения
    $db_host = "p533226.mysql.ihc.ru";
    $db_user = "p533226_1"; // Логин БД
    $db_password = "pcF8m5W7Xs"; // Пароль БД
    $db_base = "p533226_1"; // Имя БД
    $db_table = "test12"; // Имя Таблицы БД
  //  $db_table2 = "bd1";
    // Подключение к базе данных
    $mysqli = new mysqli($db_host,$db_user,$db_password,$db_base);
    $mysqli->set_charset('Utf8');
    mb_internal_encoding("UTF-8");
    // Если есть ошибка соединения, выводим её и убиваем подключение
	if ($mysqli->connect_error) {
	    die('Ошибка : ('. $mysqli->connect_errno .') '. $mysqli->connect_error);
	}

   $result1 = $mysqli->query("INSERT INTO ".$db_table." (number) VALUES ('$text')");
  // $result1 = $mysqli->query("SELECT * FROM $db_table WHERE number = $text");


    if ($result1 == true){
      //  $result2 = $mysqli->query("SELECT * FROM $db_table2 WHERE number = $number");
        //$result1 = $result1->fetch_assoc();
      //  $result2 = $result2->fetch_assoc();

        //if  ($result1['number'] == $result2['number'])
          //  require_once("services.html");
      echo "Занесено в очередь";
    }else{
    	echo "Информация не занесена в базу данных";
    }
}
?>
