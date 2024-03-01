void run( void )
{
  randomNr++; //counter used to generate random numbers

  if(IFS(0) & 0x100){ //runs during timer interrupts, main game functions
    IFSCLR(0) = 0x100; //clears interrupt flag
    TMR2 = 0x0; //resets timer

    if(menu){ //menu logic
      score = 0;
      drawMenu();
      drawDino();
      int n1;
      for(n1 = 0; n1 < 128; n1++){
        setPixel(n1, groundY, 0);
      }
      display_image(0, icon0); //displays screen data
      display_image(32, icon1);
      display_image(64, icon2);
      display_image(96, icon3);
      iconClear();
      if(getbtns() & 0x1){
        menu = 0;
      }
    }else if(gameOver){ //on gameover, freezeframe and reset values
      dinoX = 3;
      dinoY = 22;
      dinoSizeX = 8; //collider size
      dinoSizeY = 8; //collider size
      groundY = 30;
      grounded = 1;
      jumpVelo = 0;

      int i;
      for(i = 0; i < 8; i++){
        obsX[i] = -10;
        obsY[i] = 0;
        obsType[i] = 0;
      } 
      
      /* Goes back to menu when button is pressed*/
      if(getbtns() & 0x1){
        gameOver = 0;
        menu = 1;
      }
    }else{ //in game logic
      score++; //increment score each frame
      if(getbtns() & 0x4){ //jumps on button press
        jump();
      }
      if(grounded){ //while grounded sets position and velocity to ground and zero
        jumpVelo = 0;
        if(getbtns() & 0x2){ //sets ground position and hitbox size while ducking
          dinoSizeY = 5;
          dinoY = groundY - dinoSizeY;
        }else{ //while not ducking
          dinoSizeY = 8;
          dinoY = groundY - dinoSizeY;
        }
      }else{ //midair logic
        if(getbtns() & 0x2){ //fall faster while ducking
          jumpVelo += 10;
        } else { //normal fall
          jumpVelo += 2;
        }
        dinoY = (int)(dinoY + jumpVelo); //dino ascends or descends based on current velocity
      }

      if((dinoY + jumpVelo) > groundY && jumpVelo > 0){ //sets position to ground and grounded to 1 if dino would be underground next frame
        dinoY = groundY - dinoSizeY;
        grounded = 1;
      }

      int n0;
      for(n0 = 0; n0<8; n0++){ //obstacle move code, loops for each obstacle

        obsX[n0] += obsSpeed; //moves obstacle by obstacle speed
        
        static int obstacleTimer = 0;
        obstacleTimer++;
        if(obstacleTimer >= 128){ //randomizes, aka resets x value and randomizes type and position for the most distant obstacle
          randomize();
          obstacleTimer = 0;
        }
      }

      collisionCheckAll(); //checks if any obstacle is colliding with dinosaur

      int n1;
      for(n1 = 0; n1 < 128; n1++){ //draws the ground
        setPixel(n1, groundY, 0);
      }
      if(getbtns() & 0x2){//draws dino either ducking or not ducking
        drawDuck();
      }else{
        drawDino();
      }

      
      drawNumberLine(123, 1, score);
      drawObstacles();
      display_image(0, icon0); //displays screen data
      display_image(32, icon1);
      display_image(64, icon2);
      display_image(96, icon3);
      iconClear(); //clears screen data
    }
  }
}
