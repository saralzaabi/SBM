import ddf.minim.*;
import ddf.minim.analysis.*;
import ddf.minim.effects.*;
import ddf.minim.signals.*;
import ddf.minim.spi.*;
import ddf.minim.ugens.*;



import processing.sound.*;
import processing.video.*;
import org.openkinect.freenect.*;
import org.openkinect.processing.*;

import peasy.*;



PShader sobelShader;
PGraphics buffer;



Kinect kinect;


PImage depthImg;

int minDepth =  60;
int maxDepth = 860;

boolean Video1Playing = false;
boolean Video2Playing = false;

boolean Sound1Playing = false;
boolean Sound2Playing = false;

Minim minim;
Minim minim2;
AudioPlayer sea1;
AudioPlayer sea2;


float angle;


//Videos
Movie Video1; 
Movie Video2;

//Sound

SoundFile Sound1;
SoundFile Sound2;




//Fim Videos

void setup() { 
//size(1280, 720);
fullScreen();
  kinect = new Kinect(this);
  kinect.initDepth();
  angle = kinect.getTilt();


  //videos
  Video1= new Movie(this, "seavid.mp4");
  Video2 = new Movie(this, "glitchvid.mp4");

  //  if (Vid1Play==true) {
  //Video1.play();
  //   image(Video1, 0, 0, width, height);

  //  }

  //  else if (Vid1Play==false) {

  //    Video1.stop();

  //  }

  //    //videos


  //  // if (Vid2Play==true) {

  //  //   Video2.play();
  //  //   image(Video2, 0, 0, width, height);

  //  //}

  //     else if (Vid2Play==false) {

  //    Video2.stop();

  //     }



  //sound 

//  Sound1 = new SoundFile (this, "calmsea.aif");
//  Sound2 = new SoundFile(this, "glitchsound.aif");

minim = new Minim(this);

sea1 = minim.loadFile("calmsea.mp3", 2048);
  //sea1.loop();
  
  minim2 = new Minim(this);

  
  sea2= minim2.loadFile("glitchsound.mp3", 2048);



  //       if (Audio1Play==true) {
  // Sound1.play();

  //}

  //else if (Audio1Play==false) {

  // Sound1.stop();

  //}

  // if (Audio2Play==true) {

  //   Sound2.play();


  //}

  //   else if (Audio2Play==false) {

  //  Sound2.stop();

  //   }




  //  println("SFSampleRate= " + Sound1.sampleRate() + " Hz");
  //println("SFSamples= " + Sound1.frames() + " samples");
  //println("SFDuration= " + Sound1.duration() + " seconds");

  //   println("SFSampleRate= " + Sound2.sampleRate() + " Hz");
  //println("SFSamples= " + Sound2.frames() + " samples");
  //println("SFDuration= " + Sound2.duration() + " seconds");

  depthImg = new PImage(kinect.width, kinect.height);
}


//int i = 0;
//while (true) {
//  delay(1000);
//  if (Sound1.isPlaying()) {
//    i++;
//    println("File is still playing after " + i + " seconds");
//  } else {
//    break;
//  }
//}
//println("Soundfile finished playing!");



//Video1.loop();
//Video2.loop();

//Fim videos


void draw() { 
  //image(kinect.getDepthImage(), 0, 0);

  int[] depth = kinect.getRawDepth();
  int i = 0;
  float closestPoint = 800;
  int closestX=0;
  int closestY=0;
  for (int x = 0; x < kinect.width; x++) {
    for (int y = 0; y < kinect.height; y++) {
      int offset =  x + y*kinect.width;
      // Grabbing the raw depth
      int rawDepth = depth[offset];
      //println(mouseX);
      if (rawDepth >= minDepth && rawDepth <= maxDepth && x>162 && x<678) {
        depthImg.pixels[offset] = color(255);
        if (rawDepth<closestPoint) {
          closestPoint = rawDepth;
          closestX=x;
          closestY=y;
        }
      } else {
        depthImg.pixels[offset] = color(0);
      }
    }
  }

  // Draw the thresholded image
  depthImg.updatePixels();
  println(closestPoint);
  //pushStyle();



  if (closestPoint>700) {
    
   
  Video1.play();
  
   image(Video1, 0, 0, width, height);
   
   sea1.play();
   

  }  
    
if (closestPoint<700) {   
           
        Video1.stop();

          
      Video2.play();

            image(Video2, 0, 0, width, height);
            
   sea1.pause();
   sea2.play();
   
           
  //         if ( sea1.isPlaying() )
  //{
  //  sea1.pause();
  //  sea2.play();
  }
  
         
       if (closestPoint>700 &&  sea2.isPlaying()) {
         
         
         sea2.pause();
         sea1.loop();
         
       }
         
  }
  
 

  //if ( sea1.isPlaying() && sea2.isPlaying() ) 
  
  //{
    
  //  sea1.pause();
  

    
    
    

  //{
  //  // simply call loop again to resume playing from where it was paused
  //  sea1.play();
  //}

 

  


////    if (!Video1Playing) {

//      Video1.play();
//      image(Video1, 0, 0, width, height);
      
     
    

//      //tint(22, 0, 0);


//    if (closestPoint<500 && !Sound1Playing ) {

//       Sound1.play();
    
          
//    }


//    if (closestPoint>500 && Sound1Playing) {

//        Sound1.stop();
//        Sound1Playing=false; }
        
        
        
//   if (closestPoint>500 && !Sound2Playing) {
   
//      Sound2.play();
//      Sound2Playing = true; }



//          if (closestPoint<500 && !Video1Playing) {


//            Video2.stop();
            
//            Video1.play();

//            image(Video1, 0, 0, width, height);
            
//          }

   
 
 //     image(depthImg, kinect.width, 0);
  
 
       
        

            //Vid1=true;

            //Vid1Play=true;
            //Audio1Play=true;

            //Vid2=false;
            //Video1.volume(0);
            ////background(0);
            //image(Video1, 0, 0, width, height);
        
          //else {

          //  //Vid1=false;
          //  //Audio1Play=false;
          //  //Sound1.pause();
          //  //Audio1Play=false;
          //  //Vid1Play=false;

          //   Video1.stop();
          //  //image(Video2, 0, 0, width, height);
          //  //Sound1.stop();
          // //background(0);
          //}


//          if (closestPoint<600) {
//             // //tint(0, 22, 0);
//             // //Vid1=false;
//              Video1.stop();
//             // //Vid2=true;
//             // //background(0);
//             // Video1.stop(); 
//             // Sound1.stop();
//             // Vid1=false;
//             // Vid2=true;
//              Video2.play(); 
//             // Sound2.play();
//             // //Video2.volume(0);
//              image(Video2, 0, 0, width, height);

//          }

          //  else {

          //     Video2.stop();
          //   //  Sound2.stop();
          //     Video1.play();
          //   //  Sound1.play();

          //   //}

          //}




          //     popStyle();
          //  pushStyle();
          //  fill(255,0,0);
          //  pushMatrix();
          //  translate(640,0);
          //  ellipse(closestX,closestY,30,30);
          //  popMatrix();
          //  popStyle();

          //fill(0);
          //text("TILT: " + angle, 10, 20);
          //text("THRESHOLD: [" + minDepth + ", " + maxDepth + "]", 10, 36);

          //  //toogle();
          //  if (Vid1) {

          //    println("video1");
          //    Video2.stop();
          //    background(0);
          //    Video1.play();
          //    background(0);
          //    image(Video1, 0, 0, width, height);
          //  }

          //  if (Vid2) {
          //    println("video2");
          //    Video1.stop();
          //    background(0);
          //    Video2.play();
          //    image(Video2, 0, 0, width, height);
          //  }

  //}
        
 
        void movieEvent(Movie m) { 
          m.read();
        } 

        void keyPressed() {


          //  //video
          //  if (key == '1') {
          //    VideoPlaying = 1 ;
          //    println(VideoPlaying);
          //  }

          //  if (key == '2') {
          //    VideoPlaying = 2 ;
          //    println(VideoPlaying);
          //  }

          if (key == CODED) {
            if (keyCode == UP) {
              angle++;
            } else if (keyCode == DOWN) {
              angle--;
            }
            angle = constrain(angle, 0, 30);
            kinect.setTilt(angle);
          } else if (key == 'a') {
            minDepth = constrain(minDepth+10, 0, maxDepth);
          } else if (key == 's') {
            minDepth = constrain(minDepth-10, 0, maxDepth);
          } else if (key == 'z') {
            maxDepth = constrain(maxDepth+10, minDepth, 2047);
          } else if (key =='x') {
            maxDepth = constrain(maxDepth-10, minDepth, 2047);
          }
        }




        //void toogle() {

        //  if (VideoPlaying == 1) {
        //    Vid1 = true;
        //    Vid2 = false;
        //  }

        //  if (VideoPlaying == 2) {
        //    Vid1 = false;
        //    Vid2 = true;
        //  }

        //  if (VideoPlaying == 3) {
        //    Vid1 = false;
        //    Vid2 = false;
        //  }

        //  if (VideoPlaying == 4) {
        //    Vid1 = false;
        //    Vid2 = false;
        //  }
        //}



        //void movieEvent(Movie m) {
        //  m.read();
        //}
