#include <Arduino.h>
String html = R"***(
   
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>El inombrable</title>
    
        <style>
            .body{
                background-color: rgb(41, 39, 39);
            }
            .container{
                position: relative;
                top: 320px;
            }
            .header{
                position: relative;
                top: 120px;
            }
            .child{
                background-color: #79018C;
                border-radius: 20%;
                position: absolute;
                display: inline;
                left: 36%;
                width: 100px;
                height: 100px;
    
            }
            .play{
                background-color: rgba(132, 0, 255, 0);
                border: 30px;
                border-style: solid;
                border-color: rgba(255, 255, 255, 0) rgba(0, 0, 255, 0) rgba(255, 0, 0, 0) rgb(245, 245, 239);
                height: 0;
                position: absolute;
                top: 19%;
                left: 40%;
                width: 0;
            }
    
            .stop{
                background-color: #79018C;
                border: 20px;
                border-style: solid;
                border-style: double;
                border-width: 0px 0px 0px 39px;
                border-color: rgb(255, 255, 255) rgb(255, 255, 255) rgb(255, 255, 255) rgb(255, 255, 255);
                height: 43px;
                position: relative;
                top: 27%;
                display: inline-block;
                margin-left: 30%;
            }
    
            input[type=range] {
                height: 33px;
                -webkit-appearance: none;
                margin: 10px 0;
                width: 100%;
                }
    
            input[type=range]:focus {
            outline: none;
            }
            input[type=range]::-webkit-slider-runnable-track {
            width: 100%;
            height: 13px;
            cursor: pointer;
            animate: 0.2s;
            box-shadow: 0px 0px 0px #000000;
            background: #B5164B;
            border-radius: 25px;
            border: 0px solid #000101;
            }
            input[type=range]::-webkit-slider-thumb {
            box-shadow: 0px 0px 0px #000000;
            border: 0px solid #A978FF;
            height: 27px;
            width: 43px;
            border-radius: 20px;
            background: #654F5D;
            cursor: pointer;
            -webkit-appearance: none;
            margin-top: -7px;
            }
            input[type=range]:focus::-webkit-slider-runnable-track {
            background: #B5164B;
            }
            input[type=range]::-moz-range-track {
            width: 100%;
            height: 13px;
            cursor: pointer;
            animate: 0.2s;
            box-shadow: 0px 0px 0px #000000;
            background: #B5164B;
            border-radius: 25px;
            border: 0px solid #000101;
            }
            input[type=range]::-moz-range-thumb {
            box-shadow: 0px 0px 0px #000000;
            border: 0px solid #A978FF;
            height: 27px;
            width: 43px;
            border-radius: 20px;
            background: #654F5D;
            cursor: pointer;
            }
            input[type=range]::-ms-track {
            width: 100%;
            height: 13px;
            cursor: pointer;
            animate: 0.2s;
            background: transparent;
            border-color: transparent;
            color: transparent;
            }
            input[type=range]::-ms-fill-lower {
            background: #B5164B;
            border: 0px solid #000101;
            border-radius: 50px;
            box-shadow: 0px 0px 0px #000000;
            }
            input[type=range]::-ms-fill-upper {
            background: #B5164B;
            border: 0px solid #000101;
            border-radius: 50px;
            box-shadow: 0px 0px 0px #000000;
            }
            input[type=range]::-ms-thumb {
            margin-top: 1px;
            box-shadow: 0px 0px 0px #000000;
            border: 0px solid #A978FF;
            height: 27px;
            width: 43px;
            border-radius: 20px;
            background: #654F5D;
            cursor: pointer;
            }
            input[type=range]:focus::-ms-fill-lower {
            background: #B5164B;
            }
            input[type=range]:focus::-ms-fill-upper {
            background: #B5164B;
            }
            
            #vel_range{
                background-color: rgb(41, 39, 39);
    
            }
        </style>
    
    </head>
    <body class="body">
        <div class="header">
            <div class="child"> 
                <input type="button" class="play" onclick="play()">
            </div>
        </div>
    
        <div class="container">
            <input type="range" name="" id="vel_range">
        </div>
    </body>
    
    
    <script>
        var toggle = 0;
        //Toggle function: 
        function play(value){
    
            if(toggle == 0){
                document.querySelector(".play").className = "stop";
                toggle = 1
            }
            else{
                document.querySelector(".stop").className = "play";
                toggle = 0
            }
        }
    </script>
    </html>
    )***";
