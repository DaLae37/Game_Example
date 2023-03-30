//canvas Set
var canvas = document.getElementById('gameCanvas')
var context = canvas.getContext('2d')

canvas.width = 1000;
canvas.height = 1000;


//canvsss Set Finish

//canvas img Set
var player = new Image()
player.src = "img/player.png"

var arrow = new Image()
arrow.src = "img/arrow.png"

var enemy = new Image()
enemy.src= "img/shot.png"

var boss = new Image()
boss.src = "img/nuclear.png"

var heart = new Image()
heart.src = "img/heart.png"

var nuclear = new Image()
nuclear.src = "img/enemy.png"

var qShot = new Image()
qShot.src = "img/shot.png"
//canvas img Set Finish

//canvas object Set
var playerX = playerX = (canvas.width)/2;
var playerY = 600;

var bossX = 0;
var bossY = 0;

var bossHealth = 20;

var health = 5

var nuclearX = 0;
var nuclearY = 0;



//canvas object Set Finish
var shot = new Array()

for(var a = 0; a<1000+1; a++){
    shot[a] = 1;
}

var enemyX = new Array()

for(var i = 0; i<1000+1; i++){
    enemyX[i] = 3;
}

var enemyY = new Array()

for(var b = 0; b<1000+1; b++){
    enemyY[b] = 3;
}

var bossShot = new Array()

for(var k = 0; k<1000; k++){
    bossShot[k] = 2;
}


//canvas etc Set
var i = 0;
var e = 0;

var bossMoving = 25

var bossCount = 1;

var time = 0;

//canvas etc Set Finish

setInterval(init,50)


setInterval(drawTime,1000)


function init(){
       context.clearRect(0,0,canvas.width,canvas.height)
       drawPlayer()
       drawArrow()
       drawBoss()
       bossPungta()
       if(bossHealth < 10){
            bossUltimate()   
       }
       if(bossHealth < 0){
           alert("You Win !!");
           window.close()
       }
        
       heartNum()
}


function heartNum(){
    var a = 0;
    for(var i = 0; i<health; i++){
        context.drawImage(heart,a,550)
        a = a+70;
    }
    if(health == 0){
        alert("사망! "+time+"초 생");
        window.close();
    }
}

function drawTime(){
    time++;
}


function bossUltimate(){
    makeEnemy();
    drawEnemy();
}

function drawPlayer(){
    context.drawImage(player,playerX,playerY,50,50)
}

function makeEnemy(){
    var checkXY = Math.floor(Math.random()*2)
    
    if(checkXY == 0){
        // x좌표 기반
        var checkX =Math.floor(Math.random()*100+900)
        var inputY = Math.floor(Math.random()*1000)
        enemyX[checkX] = inputY
        
    }
    else if(checkXY == 1){
        var checkY = Math.floor(Math.random()*100+500)
        var inputX = Math.floor(Math.random()*1000);
        enemyY[checkY] = inputX
    
    }
}

function drawEnemy(){
    for(var a = 0; a<1000+1; a++){
        if(enemyX[a] != 3){
            context.drawImage(enemy,a,enemyX[a])
            enemyX[a-25] = enemyX[a];
            
            if(playerX > a-1 && playerX<a+51 && playerY > enemyX[a]-1 && playerY < enemyX[a]+50){
                enemyY[a-75] = enemyY[a-25];
                health--;
            }
            
            enemyX[a] = 3
        }
    }
    
    for(var b = 0; b<1000+1; b++){
        if(enemyY[b] != 3){ 
            context.drawImage(enemy,enemyY[b],b)
            enemyY[b-25] = enemyY[b];
            
            if(playerY > b-1 && playerY<b+51 && playerX > enemyY[b]-1 && playerX < enemyY[b]+50){
                enemyY[b-70] = enemyY[b-25];
                health--;
            }
            
            enemyY[b] = 3
        }
    }
    

}

function drawArrow(){
    for(i=0; i<canvas.width+1; i++){
        if(shot[i] != 1){
            if(i > bossX-1 && i+10 < bossX+130+1 && shot[i] > bossY-1 && shot[i]+25 <bossY+130){
                bossHealth--;
                console.log(bossHealth)
            }
            context.drawImage(arrow,i,shot[i])
            shot[i] = shot[i] - 35;
            
        }
        
    }
}

function drawBoss(){
    context.drawImage(boss,bossX,bossY)
    if(bossX < 1){
        bossX = 0
        bossMoving = 25
    }
    else if(bossX > 1000-81){
        bossX = 1000-80
        bossMoving = -25
    }
    
    bossX = bossX + bossMoving;
    
}

function bossPungta(){
    if(bossShot[bossX] == 2){
        bossShot[bossX] = 0    
    }

    h = 100
    for(var i = 0; i<1000; i+=40){
        
        if(bossShot[i] != 2){
            context.drawImage(qShot,i,bossShot[i])
            bossShot[i] += 50
            if(playerY > bossShot[i]-1 && playerY<bossShot[i]+51 && playerX > i-1 && playerX < i+50){
                bossShot[i] +=60
                health--;
            }
            
            if(bossShot[i] > 500){
                bossShot[i] = 2
            }
            
        }
    }
    
}
    
window.onkeydown = function(e){
    if(e.keyCode == 32){
        shot[playerX+20]= playerY;
    }
    
    if(e.keyCode == 38){
        playerY = playerY - 25;
        if(playerY < 1){
            playerY = 0;
        }
    }
    else if(e.keyCode == 40){   
        playerY = playerY + 25;
        if(playerY > canvas.height-50){
            playerY = canvas.height-50;
        }
    }
    else if(e.keyCode == 37){
        playerX = playerX - 25;
        if(playerX < 1){
            playerX = 0;
        }
    }
    else if(e.keyCode == 39){
        playerX = playerX + 25;
        if(playerX > canvas.width-100){
            playerX = canvas.width-100;
        }
    }
    
  
}