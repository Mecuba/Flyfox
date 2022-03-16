#include <Arduino.h>
String error = R"***(
    <html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Error</title>
</head>
<style>
    
    .body{
        background-color: rgb(39, 41, 41);
    }    

</style>

<body>
    
</body>
    <script>
        alert("No se pudo conectar")
    </script>
</html>
 )***"; 