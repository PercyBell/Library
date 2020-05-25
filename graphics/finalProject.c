/*
 *  FINAL PROJECT - Percy Bell
 *
 *  Key bindings:
 *  +/-        Changes field of view for perspective
 *  a          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 *  l,s,t,p,f to change flower mode
 *  SPACE and x to change time
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int axes=0;       //  Display axes
int mode=1;       //  Projection mode
int th=0;         //  Azimuth of view angle
double thT = 0;
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=38.0;   //  Size of world

int light = 1;

//modified from lighting example and past projects
int one       =   1;  // Unit value 
int distance  =   25;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   100;  // Emission intensity (%)
int ambient   =  60;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   10;  // Elevation of light

unsigned int texture[20];  //  Textures

int mtime = 50; //Maximum possible time
int time = 0; // The current time
int timea = 0; // temp time

int fmode = 0; // for flowers
int smode = 1; // for stems
int pmode = 0; // for pot
int lmode = 0; // for leaf
int tmode = 0; // for type

int look = 38; // where the eyes are
int scalelf = 1; // not important

int timeMove = 1; // turns on/off auto time

// area to store points for making models

// First bowl
// Pass into complex circle with # of circles
float bowl1[8][6] = {{0,0,0,0,0,0},
                        {0,0,0,0,0,4},
                        {0,1,0,0,0,6},
                        {0,2,0,0,0,8},
                        {0,5,0,0,0,10},
                        {0,6,0,0,0,8},
                        {0,4,0,0,0,6},
                        {0,4,0,0,0,0}};

int texB1[7] = {4,4,4,1,3,3,5};

// second bowl
float bowl2[8][6] = {{0,0,0,0,0,0},
                        {0,0,0,0,0,4},
                        {0,4,0,0,0,5},
                        {0,4,0,0,0,5.4},
                        {0,5,0,0,0,5.4},
                        {0,5,0,0,0,4.5},
                        {0,4,0,0,0,4},
                        {0,4,0,0,0,0}};

int texB2[7] = {5,5,5,5,5,5,5};

// A list of points to put leaves at
float leafPoints1[18][7] = {
   {0,0,0,0,4,0,1.5},
   {45,0,0,0,4,0,1.5},
   {90,0,0,0,4,0,1.5},
   {135,0,0,0,4,0,1.5},
   {180,0,0,0,4,0,1.5},
   {225,0,0,0,4,0,1.5},
   {270,0,0,0,4,0,1.5},
   {315,0,0,0,4,0,1.5},

   {0,0,30,0,4,0,1},
   {45,0,30,0,4,0,1},
   {90,0,30,0,4,0,1},
   {135,0,30,0,4,0,1},
   {180,0,30,0,4,0,1},
   {225,0,30,0,4,0,1},
   {270,0,30,0,4,0,1},
   {315,0,30,0,4,0,1},

   {0, 0, 0, 1, 8, -1, 0.5},
   {180, 0, 0, -0.5, 12.5, 0.5, 0.5}

};

// more leaf points
float leafPoints2[20][7] = { // for stem mode 2 only
   {0,0,0,0,4,0,1.5},
   {45,0,0,0,4,0,1.5},
   {90,0,0,0,4,0,1.5},
   {135,0,0,0,4,0,1.5},
   {180,0,0,0,4,0,1.5},
   {225,0,0,0,4,0,1.5},
   {270,0,0,0,4,0,1.5},
   {315,0,0,0,4,0,1.5},

   {0,0,30,0,4,0,1},
   {45,0,30,0,4,0,1},
   {90,0,30,0,4,0,1},
   {135,0,30,0,4,0,1},
   {180,0,30,0,4,0,1},
   {225,0,30,0,4,0,1},
   {270,0,30,0,4,0,1},
   {315,0,30,0,4,0,1},

   {0, 0, 30, 0, 0, 0, 3},
   {170, 0, 30, 0, 0, 0, 3},
   {90, 0, 30, 0, 0, 0, 3},
   {260, 0, 30, 0, 0, 0, 3}

};

// the pivot points of the stem
float stemPoints1[5][6] = {
   {0,0,0,0,0,0.15},
   {0,4,0,0,0,0.1},
   {1,8,-1,0,0,0.1},
   {-1,11,1,0,0,0.1},
   {0,15,0,0,0,0},
};

// texures of stem
int texS [4] = {5,5,5,5};

// where to put flowers and how
float flowerPoints[1][7] = {
   {0,0,0,0,15,0,1}
};

//  Macro for sin & cos in degrees from examples
#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 * from ex9
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Function to print any errors encountered from examples
 */
void errCheck(char* where)
{
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}

//************************ texture stuff from examples
void Fatal(const char* format , ...)
{
   va_list args;
   va_start(args,format);
   vfprintf(stderr,format,args);
   va_end(args);
   exit(1);
}

/*
 *  Reverse n bytes
 */
static void Reverse(void* x,const int n)
{
   int k;
   char* ch = (char*)x;
   for (k=0;k<n/2;k++)
   {
      char tmp = ch[k];
      ch[k] = ch[n-1-k];
      ch[n-1-k] = tmp;
   }
}

/*
 *  Load texture from BMP file
 */
unsigned int LoadTexBMP(const char* file)
{
   unsigned int   texture;    // Texture name
   FILE*          f;          // File pointer
   unsigned short magic;      // Image magic
   unsigned int   dx,dy,size; // Image dimensions
   unsigned short nbp,bpp;    // Planes and bits per pixel
   unsigned char* image;      // Image data
   unsigned int   off;        // Image offset
   unsigned int   k;          // Counter
   int            max;        // Maximum texture dimensions

   //  Open file
   f = fopen(file,"rb");
   if (!f) Fatal("Cannot open file %s\n",file);
   //  Check image magic
   if (fread(&magic,2,1,f)!=1) Fatal("Cannot read magic from %s\n",file);
   if (magic!=0x4D42 && magic!=0x424D) Fatal("Image magic not BMP in %s\n",file);
   //  Read header
   if (fseek(f,8,SEEK_CUR) || fread(&off,4,1,f)!=1 ||
       fseek(f,4,SEEK_CUR) || fread(&dx,4,1,f)!=1 || fread(&dy,4,1,f)!=1 ||
       fread(&nbp,2,1,f)!=1 || fread(&bpp,2,1,f)!=1 || fread(&k,4,1,f)!=1)
     Fatal("Cannot read header from %s\n",file);
   //  Reverse bytes on big endian hardware (detected by backwards magic)
   if (magic==0x424D)
   {
      Reverse(&off,4);
      Reverse(&dx,4);
      Reverse(&dy,4);
      Reverse(&nbp,2);
      Reverse(&bpp,2);
      Reverse(&k,4);
   }
   //  Check image parameters
   glGetIntegerv(GL_MAX_TEXTURE_SIZE,&max);
   if (dx<1 || dx>max) Fatal("%s image width %d out of range 1-%d\n",file,dx,max);
   if (dy<1 || dy>max) Fatal("%s image height %d out of range 1-%d\n",file,dy,max);
   if (nbp!=1)  Fatal("%s bit planes is not 1: %d\n",file,nbp);
   if (bpp!=24) Fatal("%s bits per pixel is not 24: %d\n",file,bpp);
   if (k!=0)    Fatal("%s compressed files not supported\n",file);
#ifndef GL_VERSION_2_0
   //  OpenGL 2.0 lifts the restriction that texture size must be a power of two
   for (k=1;k<dx;k*=2);
   if (k!=dx) Fatal("%s image width not a power of two: %d\n",file,dx);
   for (k=1;k<dy;k*=2);
   if (k!=dy) Fatal("%s image height not a power of two: %d\n",file,dy);
#endif

   //  Allocate image memory
   size = 3*dx*dy;
   image = (unsigned char*) malloc(size);
   if (!image) Fatal("Cannot allocate %d bytes of memory for image %s\n",size,file);
   //  Seek to and read image
   if (fseek(f,off,SEEK_SET) || fread(image,size,1,f)!=1) Fatal("Error reading data from image %s\n",file);
   fclose(f);
   //  Reverse colors (BGR -> RGB)
   for (k=0;k<size;k+=3)
   {
      unsigned char temp = image[k];
      image[k]   = image[k+2];
      image[k+2] = temp;
   }

   //  Sanity check
   //ErrCheck("LoadTexBMP");
   //  Generate 2D texture
   glGenTextures(1,&texture);
   glBindTexture(GL_TEXTURE_2D,texture);
   //  Copy image
   glTexImage2D(GL_TEXTURE_2D,0,3,dx,dy,0,GL_RGB,GL_UNSIGNED_BYTE,image);
   if (glGetError()) Fatal("Error in glTexImage2D %s %dx%d\n",file,dx,dy);
   //  Scale linearly when image size doesn't match
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

   //  Free image memory
   free(image);
   //  Return texture name
   return texture;
}
// *************** textures over

// **************** lights camera action
// Light stuff from examples

// vertex for light from examples
static void VertexLight(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

// light ball from examples
static void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         VertexLight(th,ph);
         VertexLight(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}

// ****************** lights over

/*
 *  Set projection from ex9
 */
static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (mode)
      gluPerspective(fov,asp,dim/4,4*dim);
   //  Orthogonal projection
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

// A matrix print function for debugging 
void printM(int r,int c,float M[r][c]) {
   for (int i = 0; i < r; i++) {
      for (int j = 0; j < c; j++) {
         printf("%f /", M[i][j]);
      }
      printf("\n");
   }
}

// Take AcrossB if both are 1x3 vectors
void crossV(float A[3], float B[3], float C[3]) {

   C[0] = A[1]*B[2] - A[2]*B[1];
   C[1] = A[2]*B[0] - A[0]*B[2];
   C[2] = A[0]*B[1] - A[1]*B[0];

}

// Calculate normal vector from 3 points
void norm(float p1[3], float p2[3], float p3[3], float res[3]) {
   float a[3];
   float b[3];

   float x1 = p1[0];
   float x2 = p2[0];
   float x3 = p3[0];

   float y1 = p1[1];
   float y2 = p2[1];
   float y3 = p3[1];

   float z1 = p1[2];
   float z2 = p2[2];
   float z3 = p3[2];

   a[0] = x2-x1;
   a[1] = y2-y1;
   a[2] = z2-z1;

   b[0] = x3-x1;
   b[1] = y3-y1;
   b[2] = z3-z1;

   crossV(a,b,res);

}

/*
*  A matrix multiplacation functin (AXB)
*  found from https://www.geeksforgeeks.org/c-program-multiply-two-matrices/
*  coppied because I didnt want to spend too much time on this
*/
void multiply(int m1, int m2, float mat1[][m2],
              int n1, int n2, float mat2[][n2],float res[m1][n2])
{
    int x, i, j;
    // float res[m1][n2];
    for (i = 0; i < m1; i++) {
        for (j = 0; j < n2; j++) {
            res[i][j] = 0;
            for (x = 0; x < m2; x++) {
                *(*(res + i) + j) += *(*(mat1 + i) + x) *
                                     *(*(mat2 + x) + j);
            }
        }
    }
    /*
    for (i = 0; i < m1; i++) {
        for (j = 0; j < n2; j++) {
            printf("%f ", *(*(res + i) + j));
        }
        printf("\n");
    }
    */
    //printf("------------\n");
    //printM(m1,n2,res);
    //printf("\n");
}

// The rotate function
void rotate(char dim, int a, float matrix[4][4]) {
   // fix shit
   for (int i=0;i<4;i++) {
      for (int j = 0;j<4;j++) {
         matrix[i][j] = 0;
      }
   }

   matrix[0][0] = 1;
   matrix[1][1] = 1;
   matrix[2][2] = 1;
   matrix[3][3] = 1;

   //printM(4,4,matrix);

   // To rotate in the x dim
   if (dim == 'x') {
      matrix[1][1] = Cos(a);
      matrix[1][2] = -Sin(a);
      matrix[2][1] = Sin(a);
      matrix[2][2] = Cos(a);
   }

   else if (dim == 'y') {
      matrix[0][0] = Cos(a);
      matrix[0][2] = Sin(a);
      matrix[2][0] = -Sin(a);
      matrix[2][2] = Cos(a);
   }

   else if (dim == 'z') {
      matrix[0][0] = Cos(a);
      matrix[0][1] = -Sin(a);
      matrix[1][0] = Sin(a);
      matrix[1][1] = Cos(a);
   }
   //printf(">>>>>>>>>>>\n");
   //printM(4,4,matrix);
   //printf(">>>>>>>>>>>\n");
}

// A translation matrix funciton 
void translate(float x, float y, float z, float matrix[4][4]) {
   // fix shit
   for (int i=0;i<4;i++) {
      for (int j = 0;j<4;j++) {
         matrix[i][j] = 0;
      }
   }

   matrix[0][0] = 1;
   matrix[1][1] = 1;
   matrix[2][2] = 1;
   matrix[3][3] = 1;

   matrix[0][3] = x;
   matrix[1][3] = y;
   matrix[2][3] = z;
}

// draw a circle and transform it to a place
void circleDraw(float r, int anX, int anZ,float circle[24][4],float x1,float y1,float z1){
   int angle = 0;
   float x = 0;
   float z = 0;

   for (int i=0;i<24;i ++) {
      x = Sin(angle)*r;
      z = Cos(angle)*r;
      circle[i][0] = x; // x 
      circle[i][2] = z; // x
      circle[i][1] = 0; // y
      circle[i][3] = 0; // homogenious
      angle += 15;
      //printf("%f\n",x);
   }
   
   float rotateX [4][4];
   float rotateZ [4][4];
   float trans[4][4];

   rotate('x', anX,rotateX);
   rotate('z', anZ,rotateZ);
   translate(x1,y1,z1,trans);
   //printM(4,4,trans);

   //printM(4,4,rotateX);

   float res[24][4];
   multiply(24,4,circle,4,4,rotateX,res);
   //printM(24,4, circle);
   //printf("-------------\n");

   for (int i=0;i<24;i++) {
      circle[i][0] += x1;
      circle[i][1] += y1;
      circle[i][2] += z1;
   }
   //printM(24,4, circle);

   float res1[24][4];
   multiply(24,4,circle,4,4,rotateZ,res1);

   for (int i=0;i<24;i++) {
      for (int j = 0;j<4;j++) {
         circle[i][j] = res1[i][j];
      }
   }

   float res2[24][4];
   //printM(24,4,circle);
   multiply(24,4,circle,4,4,trans,res2);
   //printM(4,4,trans);
   //printf("\n");
   //printM(24,4,res2);

   for (int i=0;i<24;i++) {
      for (int j = 0;j<4;j++) {
         circle[i][j] = res2[i][j];
      }
   }

}

// display normal vector for p1,2,3
void nVec(float p1[3], float p2[3], float p3[3], float x, float y, float z) {
   glPushMatrix();

   glTranslated(x,y,z);

   glBegin(GL_LINE_STRIP);
   glVertex3d(0,0,0);
   float norml[3];
   norm(p1,p2,p3,norml);
   glVertex3d(norml[0],norml[1],norml[2]);
   glPopMatrix();
}

// make leaf 1
void leaf1(int az, int pt, int ro, float x, float y, float z, float s) {
   glPushMatrix();
   float p1[3];
   float p2[3];
   float p3[3];
   float nor[3];

   if (scalelf) {
      glScaled(1+(time/100.0),1+(time/100.0),1+(time/100.0));
   }

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[2]);

   glTranslated(x,y,z);
   glRotated(az,0,1,0);
   glRotated(pt,1,0,0);
   glRotated(ro,0,0,1);
   glScaled(s + time/100.0,s,s);

   // I set the normal vector for the entire shape onece because it gives it a very smooth look, which I think looks realistic for leaves
   // I also decided to make the texture for the leaves 2D only because then the light highlights both sides of the leaf when it passes
   // I think that looks fairly realisitc
   glColor3f(0,1,0);
   glBegin(GL_POLYGON);
   p1[0] = 0;
   p1[1] = 0;
   p1[2] = 0;

   p2[0] = 1;
   p2[1] = 1;
   p2[2] = -1;

   p3[0] = 1;
   p3[1] = 1;
   p3[2] = 1;

   norm(p1,p2,p3,nor);
   glNormal3f(nor[0],nor[1],nor[2]);
   //nVec(p1,p2,p3,0,-5,0);
   //printf("%f,%f,%f\n",nor[0],nor[1],nor[2]);

   glTexCoord2f(0.5,0.0);
   glVertex3d(0,0,0);

   glTexCoord2f(0.0,0.3);
   glVertex3d(1,1,-1);

   glTexCoord2f(1.0,0.3);
   glVertex3d(1,1,1);

   //glVertex3d(0,0,0);
   //glVertex3d(1,1,1);
   glEnd();

   glBegin(GL_POLYGON);

   glTexCoord2f(1.0,0.3);
   glVertex3d(1,1,1);

   glTexCoord2f(1.0,0.5);
   glVertex3d(3,1.5,1.3);

   glTexCoord2f(0.0,0.5);
   glVertex3d(3,1.5,-1.3);

   glTexCoord2f(0.0,0.3);
   glVertex3d(1,1,-1);

   //glTexCoord2f(1.0,0.6);
   //glVertex3d(3,1.5,-1.3);

   glEnd();

   glBegin(GL_POLYGON);
   glTexCoord2f(1.0,0.5);
   glVertex3d(3,1.5,-1.3);

   glTexCoord2f(0.5,1.0);
   glVertex3d(4+(time/100.0),1,0);

   glTexCoord2f(0.0,0.5);
   glVertex3d(3,1.5,1.3);
   glEnd();

   glDisable(GL_TEXTURE_2D);

   glPopMatrix();
}

// Makes leaf 2 (spike leaf)
void leaf2(int az, int pt, int ro, float x, float y, float z, float s) {
   // initialize for normals
   float p1[3];
   float p2[3];
   float p3[3];
   float nor[3];

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[7]);

   glPushMatrix();

   glTranslated(x,y,z);
   glRotated(az,0,1,0);
   glRotated(pt,1,0,0);
   glRotated(ro,0,0,1);
   glScaled(s + time/100.0,s,s);

   glColor3f(0,1,0);
   glBegin(GL_POLYGON);

   // Points for normal vector
   p1[0] = 0;
   p1[1] = 0;
   p1[2] = 0;

   p2[0] = 1;
   p2[1] = 1;
   p2[2] = -1;

   p3[0] = 1;
   p3[1] = 1;
   p3[2] = 1;

   // draw polygons

   glBegin(GL_POLYGON);
   norm(p1,p2,p3,nor);
   glNormal3f(nor[0],nor[1],nor[2]);

   glTexCoord2f(0.5,0.0);
   glVertex3d(0,0,0);
   glTexCoord2f(0.25,1/7.0);
   glVertex3d(1,1,-1);
   glTexCoord2f(0.0,3/7.0);
   glVertex3d(3,2.5,-2);
   glTexCoord2f(0.25,2.5/7.0);
   glVertex3d(2.5,2,-1);

   glEnd();

   glBegin(GL_POLYGON);
   glTexCoord2f(0.5,0.0);
   glVertex3d(0,0,0);
   glTexCoord2f(0.75,1/7.0);
   glVertex3d(1,1,1);
   glTexCoord2f(1.0,3/7.0);
   glVertex3d(3,2.5,2);
   glTexCoord2f(0.75,2.5/7.0);
   glVertex3d(2.5,2,1);

   glEnd();

   glBegin(GL_POLYGON);
   glTexCoord2f(0.5,0.0);
   glVertex3d(0,0,0);
   glTexCoord2f(0.75,2.5/7.0);
   glVertex3d(2.5,2,1);
   glTexCoord2f(0.25,2.5/7.0);
   glVertex3d(2.5,2,-1);

   glEnd();

   glBegin(GL_POLYGON);
   glTexCoord2f(0.75,2.5/7.0);
   glVertex3d(2.5,2,1);
   glTexCoord2f(0.875,5/7.0);
   glVertex3d(5,2,1.5);
   glTexCoord2f(0.625,4.2/7.0);
   glVertex3d(4.2,2,0.5);

   glEnd();

   glBegin(GL_POLYGON);
   glTexCoord2f(0.25,2.5/7.0);
   glVertex3d(2.5,2,-1);
   glTexCoord2f(0.125,5/7.0);
   glVertex3d(5,2,-1.5);
   glTexCoord2f(0.375,4.2/7.0);
   glVertex3d(4.2,2,-0.5);

   glEnd();

   glBegin(GL_POLYGON);
   glTexCoord2f(0.75,2.5/7.0);
   glVertex3d(2.5,2,1);
   glTexCoord2f(0.25,2.5/7.0);
   glVertex3d(2.5,2,-1);
   glTexCoord2f(0.375,4.2/7.0);
   glVertex3d(4.2,2,-0.5);
   glTexCoord2f(0.625,4.2/7.0);
   glVertex3d(4.2,2,0.5);

   glEnd();

   glBegin(GL_POLYGON);
   glTexCoord2f(0.375,4.2/7.0);
   glVertex3d(4.2,2,-0.5);
   glTexCoord2f(0.625,4.2/7.0);
   glVertex3d(4.2,2,0.5);
   glTexCoord2f(0.5,1.0);
   glVertex3d(7,1,0);

   glEnd();

   glDisable(GL_TEXTURE_2D);

   glPopMatrix();
}

// make petals for flowers
void petal(int ang) {

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[0]);

   glPushMatrix();

   float tX = ((4.0/6.0)/(50.0))*time;
   //printf("%f\n", tX);

   glRotated(ang,0,1,0);

   // for the normal vectors
   float polys[9][3] = {{0,0,0}, {2,1,1}, {2,1,-1},
                        {2,1,1}, {2*Sin(time-20)+2,2*Cos(time*2-20)+1,1.5},{2*Sin(time-20)+2,2*Cos(time*2-20)+1,-1.5},
                        {2*Sin(time-20)+2,2*Cos(time*2-20)+1,1.5}, {4.47*Sin(time*2-25)+2,4.47*Cos(time*2-25)+1,0} ,{2*Sin(time-20)+2,2*Cos(time*2-20)+1,-1.5}};

   glBegin(GL_POLYGON);
   float nor[3];
   norm(polys[0],polys[1],polys[2],nor);
   glNormal3f(-nor[0],-nor[1], -nor[2]);
   //nVec(polys[0],polys[1],polys[2],0,0,0);

   glTexCoord2f((1/6.0)+tX,0.0);
   glVertex3d(0,0,0);

   glTexCoord2f(0.0+tX,0.3);
   glVertex3d(2,1,1);

   glTexCoord2f(2/6.0+tX,0.3);
   glVertex3d(2,1,-1);

   glEnd();

   norm(polys[3],polys[4],polys[5],nor);
   glNormal3f(-nor[0],-nor[1], -nor[2]);
   //nVec(polys[3],polys[4],polys[5],0,0,0);

   glBegin(GL_POLYGON);
   glTexCoord2f(0.0+tX,0.3);
   glVertex3d(2,1,1);

   glTexCoord2f(0.0+tX,0.7);
   glVertex3d(2*Sin(time-20)+2,2*Cos(time*2-20)+1,1.5);

   glTexCoord2f(2/6.0+tX,0.7);
   glVertex3d(2*Sin(time-20)+2,2*Cos(time*2-20)+1,-1.5);

   glTexCoord2f(2/6.0+tX,0.3);
   glVertex3d(2,1,-1);
   glEnd();

   norm(polys[6],polys[7],polys[8],nor);
   glNormal3f(-nor[0],-nor[1], -nor[2]);
   //nVec(polys[6],polys[7],polys[8],0,0,0);

   glBegin(GL_POLYGON);
   glTexCoord2f(0.0+tX,0.7);
   glVertex3d(2*Sin(time-20)+2,2*Cos(time*2-20)+1,1.5);
   glTexCoord2f(1/6.0+tX,1.0);
   glVertex3d(4.47*Sin(time*2-25)+2,4.47*Cos(time*2-25)+1,0);
   glTexCoord2f(2/6.0+tX,0.7);
   glVertex3d(2*Sin(time-20)+2,2*Cos(time*2-20)+1,-1.5);
   glEnd();

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}

// create flower 1
void flower1(int az, int pt, int ro, float x, float y, float z) {
   glPushMatrix();

   glTranslated(x,y,z);
   glRotated(az,0,1,0);
   glRotated(pt,1,0,0);
   glRotated(ro,0,0,1);

   int ang = 0;
   for (int i = 0; i < 8; i++) {
      petal(ang);
      ang += 45;
   }
   glPopMatrix();
}

// create flower 2
void flower2(int az, int pt, int ro, float x, float y, float z) {
   glPushMatrix();

   //glColor3f(1,1,1);
   glTranslated(x,y,z);
   glRotated(az,0,1,0);
   glRotated(pt,1,0,0);
   glRotated(ro,0,0,1);
   glScaled(0.5+(time/100.0),0.5+(time/100.0),0.5+(time/100.0));

   int ang = 0;
   for (int i = 0; i < 5; i++) {
      petal(ang);
      ang += 72;
   }
   glPopMatrix();
}

// take two circles and merge them into a cylender ish but different shape
void circT(float x1, float y1, float z1, int aX1, int aZ1, float r1, float x2, float y2, float z2, int aX2, int aZ2, float r2,int t) {
   float circ1 [24][4];
   float circ2 [24][4];

   circleDraw(r1,aX1,aZ1,circ1,x1,y1,z1);
   circleDraw(r2,aX2,aZ2,circ2,x2,y2,z2);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[t]);

   glBegin(GL_LINE_STRIP);
   for (int j = 0;j<24;j++) {
      glVertex3d(circ1[j][0],circ1[j][1],circ1[j][2]);
   }
   glVertex3d(circ1[0][0],circ1[0][1],circ1[0][2]);
   glEnd();

   glBegin(GL_LINE_STRIP);
   for (int j = 0;j<24;j++) {
      glVertex3d(circ2[j][0],circ2[j][1],circ2[j][2]);
   }
   glVertex3d(circ2[0][0],circ2[0][1],circ2[0][2]);
   glEnd();

   glBegin(GL_POLYGON);
   float nor[3];

   float point1[3] = {circ1[0][0],circ1[0][1],circ1[0][2]};
   float point2[3] = {circ2[0][0],circ2[0][1],circ2[0][2]};
   float point3[3] = {circ1[1][0],circ1[1][1],circ1[1][2]};

   norm(point1,point2,point3,nor);
   glNormal3f(-nor[0],-nor[1],-nor[2]);

   int c1 = 2;
   int c2 = 1;

   glColor3f(0.5,1,1);

   glTexCoord2f(0.0,0.0);
   glVertex3d(point1[0],point1[1],point1[2]);
   glTexCoord2f(0.0,1.0);
   glVertex3d(point2[0],point2[1],point2[2]);
   glTexCoord2f(1/24.0,0.0);
   glVertex3d(point3[0],point3[1],point3[2]);
   //glVertex3d(point1[0],point1[1],point1[2]);

   glEnd();
   //glColor3f(1,1,1);

   float tex[3][2];

   tex[0][0] = 0.0;
   tex[0][1] = 0.0;

   tex[1][0] = 0.0;
   tex[1][1] = 1.0;

   tex[2][0] = 1/24.0;
   tex[2][1] = 0.0;

   int h = 1;

   for (int i = 0; i<45; i++) {
      //glColor3f(1,1,i%2);
      point1[0] = point2[0];
      point1[1] = point2[1];
      point1[2] = point2[2];

      point2[0] = point3[0];
      point2[1] = point3[1];
      point2[2] = point3[2];

      tex[0][0] = tex[1][0];
      tex[0][1] = tex[1][1];

      tex[1][0] = tex[2][0];
      tex[1][1] = tex[2][1];

      if (i % 2 == 1) {
         point3[0] = circ1[c1][0];
         point3[1] = circ1[c1][1];
         point3[2] = circ1[c1][2];
         h = -1;
         c1++;
         tex[2][0] += 1/24.0;
         tex[2][1] = 0.0;
      }
      else {
         point3[0] = circ2[c2][0];
         point3[1] = circ2[c2][1];
         point3[2] = circ2[c2][2];
         h = 1;
         c2++;
         tex[2][0] += 1/24.0;
         tex[2][1] = 1.0;
      }
      glBegin(GL_POLYGON);
      norm(point1,point2,point3,nor);

      glNormal3f(h*nor[0],h*nor[1],h*nor[2]);
      //glColor3f(1,0,1);
      glTexCoord2f(tex[0][0],tex[0][1]);
      glVertex3d(point1[0],point1[1],point1[2]);
      glTexCoord2f(tex[1][0],tex[1][1]);
      glVertex3d(point2[0],point2[1],point2[2]);
      glTexCoord2f(tex[2][0],tex[2][1]);
      glVertex3d(point3[0],point3[1],point3[2]);
      glEnd();

   }

   point1[0] = circ1[23][0];
   point1[1] = circ1[23][1];
   point1[2] = circ1[23][2];

   point2[0] = circ2[23][0];
   point2[1] = circ2[23][1];
   point2[2] = circ2[23][2];

   point3[0] = circ1[0][0];
   point3[1] = circ1[0][1];
   point3[2] = circ1[0][2];

   tex[0][0] = tex[1][0];
   tex[0][1] = tex[1][1];

   tex[1][0] = tex[2][0];
   tex[1][1] = tex[2][1];

   tex[2][0] += 1/24.0;
   tex[2][1] = 0.0;

   glBegin(GL_POLYGON);
   norm(point1,point2,point3,nor);
   glNormal3f(-nor[0],-nor[1],-nor[2]);

   //glColor3f(1,1,1);
   glTexCoord2f(tex[0][0],tex[0][1]);
   glVertex3d(point1[0],point1[1],point1[2]);
   glTexCoord2f(tex[1][0],tex[1][1]);
   glVertex3d(point2[0],point2[1],point2[2]);
   glTexCoord2f(tex[2][0],tex[2][1]);
   glVertex3d(point3[0],point3[1],point3[2]);
   glEnd();

   point1[0] = circ2[23][0];
   point1[1] = circ2[23][1];
   point1[2] = circ2[23][2];

   point2[0] = circ1[0][0];
   point2[1] = circ1[0][1];
   point2[2] = circ1[0][2];

   point3[0] = circ2[0][0];
   point3[1] = circ2[0][1];
   point3[2] = circ2[0][2];

   tex[0][0] = tex[1][0];
   tex[0][1] = tex[1][1];

   tex[1][0] = tex[2][0];
   tex[1][1] = tex[2][1];

   tex[2][0] += 1/24.0;
   tex[2][1] = 1.0;

   glBegin(GL_POLYGON);
   norm(point1,point2,point3,nor);
   glNormal3f(nor[0],nor[1],nor[2]);

   //glColor3f(1,1,0);
   glTexCoord2f(tex[0][0],tex[0][1]);
   glVertex3d(point1[0],point1[1],point1[2]);
   glTexCoord2f(tex[1][0],tex[1][1]);
   glVertex3d(point2[0],point2[1],point2[2]);
   glTexCoord2f(tex[2][0],tex[2][1]);
   glVertex3d(point3[0],point3[1],point3[2]);
   glEnd();
   glDisable(GL_TEXTURE_2D);

}

// create a shape out of cirlces which are strung together 
void complexCirc(int numC, float circles[numC][6],int ntex, int tex[ntex]) {
   float x1;
   float y1;
   float z1;
   int aX1;
   int aZ1;
   float r1;

   float x2;
   float y2;
   float z2;
   int aX2;
   int aZ2;
   float r2;

   for (int i = 0; i<numC-1;i++) {
      x1 = circles[i][0];
      y1 = circles[i][1];
      z1 = circles[i][2];

      aX1 = circles[i][3];
      aZ1 = circles[i][4];
      r1 = circles[i][5];

      x2 = circles[i+1][0];
      y2 = circles[i+1][1];
      z2 = circles[i+1][2];

      aX2 = circles[i+1][3];
      aZ2 = circles[i+1][4];
      r2 = circles[i+1][5];

      circT(x1,y1,z1,aX1,aZ1,r1,x2,y2,z2,aX2,aZ2,r2,tex[i]);
   } 
}

// The stem design
void stem1(int lenp, float points[lenp][6], int lenf, float flowers[lenf][7], int lenl, float leaves[lenl][7], float s1) {
   glPushMatrix();

   glScaled(s1,s1,s1);

   glPushMatrix();

   glColor3f(0,0.5,0.2);
   double s = time/100.00;
   glScaled(1+s,1+s,1+s);

   complexCirc(lenp,points,lenp-1,texS);

   glPopMatrix();

   for (int j = 0; j < lenf; j++) { // draw one of two flowers
      if (fmode == 1) {
         flower2(flowers[j][0],flowers[j][1],flowers[j][2],flowers[j][3]*(1+s),flowers[j][4]*(1+s),flowers[j][5]*(1+s));
      }
      else {
         flower1(flowers[j][0],flowers[j][1],flowers[j][2],flowers[j][3]*(1+s),flowers[j][4]*(1+s),flowers[j][5]*(1+s));
      }
   }

   glPushMatrix();

   glScaled(1,1,1); // draw the leaves for the plant and scale them if needed

   if (tmode != 1) {
      for (int k = 0; k < lenl ; k++) {
         if (lmode == 0) {
            leaf1(leaves[k][0],leaves[k][1],leaves[k][2],leaves[k][3],leaves[k][4],leaves[k][5]*(1+s),leaves[k][6]);
         }
         else {
            leaf2(leaves[k][0],leaves[k][1],leaves[k][2],leaves[k][3]*(1+s),leaves[k][4]*(1+s),leaves[k][5]*(1+s),leaves[k][6]);
         }
      }
   }

   glPopMatrix();

   glPopMatrix();
}

void stem2(int lenp, float points[lenp][6], int lenf, float flowers[lenf][7], int lenl, float leaves[lenl][7], float s1) {
   glPushMatrix();

   glScaled(s1/1.5,s1/1.5,s1/1.5);

   glPushMatrix();

   glColor3f(0,0.5,0.2);
   double s = time/100.00;
   glScaled(1+s,1+s,1+s);

   // draw lots of flowers and stems

   int az = 0;
   int pt = 20;

   for (int i = 0; i < 12; i++) {

      glPushMatrix();

      //glTranslated(x,y,z);
      glRotated(az,0,1,0);
      glRotated(pt,1,0,0);
      //glRotated(ro,0,0,1);

      if (i % 2 == 0) {
         pt += 10;
      }
      else {
         pt -= 10;
      }

      complexCirc(lenp,points,lenp-1,texS);

      glPushMatrix();

      glScaled(0.25,0.25,0.25);

      for (int j = 0; j < lenf; j++) { // draw one of two flowers
         if (fmode == 1) {
            flower2(flowers[j][0],flowers[j][1],flowers[j][2],flowers[j][3]*4,flowers[j][4]*4,flowers[j][5]*(1+s));
         }
         else {
            flower1(flowers[j][0],flowers[j][1],flowers[j][2],flowers[j][3]*4,flowers[j][4]*4,flowers[j][5]*(1+s));
         }
      }

      glPopMatrix();



      glPopMatrix();

      az += 30;
      //pt += 2;

   }

   az = 5;

   pt = 45;

   for (int i = 0; i < 12; i++) {

      glPushMatrix();

      //glTranslated(x,y,z);
      glRotated(az,0,1,0);
      glRotated(pt,1,0,0);
      //glRotated(ro,0,0,1);

      complexCirc(lenp,points,lenp-1,texS);

      glPushMatrix();

      glScaled(0.25,0.25,0.25);

      for (int j = 0; j < lenf; j++) { // draw one of two flowers
         if (fmode == 1) {
            flower2(flowers[j][0],flowers[j][1],flowers[j][2],flowers[j][3]*4,flowers[j][4]*4,flowers[j][5]*(1+s));
         }
         else {
            flower1(flowers[j][0],flowers[j][1],flowers[j][2],flowers[j][3]*4,flowers[j][4]*4,flowers[j][5]*(1+s));
         }
      }

      glPopMatrix();

      glPopMatrix();

      az += 30;
      //pt += 2;

   }

   glPopMatrix();

   glPushMatrix();

   glScaled(1,1,1); // draw the leaves for the plant and scale them if needed

   if (tmode != 1) {
      for (int k = 0; k < lenl ; k++) {
         if (lmode == 0) {
            leaf1(leaves[k][0],leaves[k][1],leaves[k][2],leaves[k][3],leaves[k][4],leaves[k][5]*(1+s),leaves[k][6]);
         }
         else {
            leaf2(leaves[k][0],leaves[k][1],leaves[k][2],leaves[k][3]*(1+s),leaves[k][4]*(1+s),leaves[k][5]*(1+s),leaves[k][6]);
         }
      }
   }

   glPopMatrix();



   glPopMatrix();

}

void bamboo() {

   glPushMatrix();

   double s = time/100.00;

   glScaled(1+(s/10.5),1+(s),1+(s/10.5));

   int texb[5] = {6,6,6,6,6};

   // Larger bamboo
   float bamb[6][6] = {
      {-1,0.01,0,0,0,0},
      {-1,0.01,0,0,0,1*(1 + time/100.0)},
      {-1,0.11,0,0,0,0.7*(1 + time/100.0)},
      {-1,4.91,0,0,0,0.7*(1 + time/100.0)},
      {0-1,5.1,0,0,0,1*(1 + time/100.0)},
      {0-1,5.1,0,0,0,0}
   };

   // Smaller bamboo
   float bamb2[6][6] = {
      {3,0.01,4,0,0,0},
      {3,0.01,4,0,0,1*(1 + time/100.0)},
      {3,0.11,4,0,0,0.7*(1 + time/100.0)},
      {3,4.91,4,0,0,0.7*(1 + time/100.0)},
      {3,5.1,4,0,0,1*(1 + time/100.0)},
      {3,5.1,4,0,0,0}
   };

   //draw bamboo large with all its segemts and leaves
   complexCirc(6,bamb,5,texb);

   for (int i = 0; i< 4; i++) {
      for (int j = 0; j<6;j++) {
         bamb[j][1] += 5;
      }
      complexCirc(6,bamb,5,texb);
   }

   if (lmode == 0) {
      leaf1(20,0,0,-1,4.0/(1+s),0,1.5);
      leaf1(180,0,0,-1,10.0/(1+s),0,1);
      leaf1(90,0,0,-1,11.0/(1+s),0,1.1);

      leaf1(-30,0,0,-1,17.0/(1+s),0,0.5);
      leaf1(45,0,0,-1,20.0/(1+s),0,0.4);

      //leaf1(180,0,0,1,4,-1,1);
   }
   else {
      leaf2(20,0,0,-1,4.0,0,1.5);
      leaf2(180,0,0,-1,10.0,0,1);
      leaf2(90,0,0,-1,11.0,0,1.1);

      leaf2(-30,0,0,-1,17.0,0,0.5);
      leaf2(45,0,0,-1,20.0,0,0.4);
   }

   // now draw bamboo 2
   glPushMatrix();
   glScaled(0.5,0.5,0.5);

   for (int i = 0; i< 4; i++) {
      for (int j = 0; j<6;j++) {
         bamb2[j][1] += 5;
         bamb2[j][0] = 4;
         bamb2[j][2] = 3;
      }
      complexCirc(6,bamb2,5,texb);
   }

   glPopMatrix();

   glPopMatrix();
}

// The ground function
// Basically a texured box but it looks nice
void ground(){
   glPushMatrix();

   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[8]);

   glColor3f(1,1,1);

   glBegin(GL_POLYGON);
   glNormal3f(0,1,0);

   glTexCoord2f(0,0);
   glVertex3d(15,0,15);
   glTexCoord2f(4,0);
   glVertex3d(-15,0,15);
   glTexCoord2f(4,4);
   glVertex3d(-15,0,-15);
   glTexCoord2f(0,4);
   glVertex3d(15,0,-15);

   glEnd();

   glBindTexture(GL_TEXTURE_2D,texture[9]);

   glBegin(GL_POLYGON);
   glNormal3f(0,-1,0);
   glTexCoord2f(0,0);
   glVertex3d(16,-1,16);
   glTexCoord2f(1,0);
   glVertex3d(-16,-1,16);
   glTexCoord2f(1,1);
   glVertex3d(-16,-1,-16);
   glTexCoord2f(0,1);
   glVertex3d(16,-1,-16);

   glEnd();

   glBegin(GL_POLYGON);
   glNormal3f(0,1,1);
   glTexCoord2f(0,0);
   glVertex3d(16,-1,16);
   glTexCoord2f(1,0);
   glVertex3d(-16,-1,16);
   glTexCoord2f(1,1);
   glVertex3d(-15,0,15);
   glTexCoord2f(0,1);
   glVertex3d(15,0,15);

   glEnd();

   glBegin(GL_POLYGON);
   glNormal3f(-1,1,0);
   glTexCoord2f(0,0);
   glVertex3d(-16,-1,16);
   glTexCoord2f(1,0);
   glVertex3d(-16,-1,-16);
   glTexCoord2f(1,1);
   glVertex3d(-15,0,-15);
   glTexCoord2f(0,1);
   glVertex3d(-15,0,15);

   glEnd();

   glBegin(GL_POLYGON);
   glNormal3f(0,1,-1);
   glTexCoord2f(0,0);
   glVertex3d(-16,-1,-16);
   glTexCoord2f(1,0);
   glVertex3d(16,-1,-16);
   glTexCoord2f(1,1);
   glVertex3d(15,0,-15);
   glTexCoord2f(0,1);
   glVertex3d(-15,0,-15);

   glEnd();

   glBegin(GL_POLYGON);
   glNormal3f(1,1,0);
   glTexCoord2f(0,0);
   glVertex3d(16,-1,16);
   glTexCoord2f(1,0);
   glVertex3d(16,-1,-16);
   glTexCoord2f(1,1);
   glVertex3d(15,0,-15);
   glTexCoord2f(0,1);
   glVertex3d(15,0,15);

   glEnd();

   glDisable(GL_TEXTURE_2D);


   glPopMatrix();
}

// Function to make vine
void vine(){
   glPushMatrix();

   glScaled(0.5,0.5,0.5);

   // Set of vine points and leaf placement
   float vine[12][6] = {
      {0,0,0,0,0,0},

      {0,0,0,0,0,3},
      {-1,.01,0,0,0,1},
      {0,2,1,0,0,1},
      {1,3,0,0,0,.5},
      {0,4,-1,0,0,1},
      {-1,5,0,0,0,1},
      {0,6,1,0,0,1},
      {1,7,0,0,0,.5},
      {0,8,-1,0,0,1},
      {-1,9,0,0,0,1},

      {0,9,0,0,0,0}
   };

   float leafPz[5][7] =  {
      {10,0,0,0,1,0,1},
      {50,0,0,0,1,0,1},
      {190,0,0,0,1,0,1},
      {260,0,0,0,1,0,1},
      {315,0,0,0,1,0,1}
   };

   // changes vine if stem move is active
   if (smode == 0) {
      for (int v = 0;v<12;v++) {
         vine[v][0] = vine[v][0]*2.5;
         vine[v][2] = vine[v][0]*2.5;
         leafPz[v][3] = -vine[v][0];
         leafPz[v][5] = -vine[v][2];
      }
      //leafPz[1][6] = 0;
      //leafPz[3][6] = 0;
   }

   // textures
   int texV[11] = {10,10,10,10,10,10,10,10,10,10,10};
   int grow = time/5;
   float temph = 5;

   // Update values in vine if time is right
   for (int i = 2; i < grow; i++) {
      vine[i][1] = temph + vine[i-1][1];
      vine[i][5] = vine[i-1][5] - 0.5;

      leafPz[0][4] = vine[2][1];
      leafPz[1][4] = vine[3][1];
      leafPz[2][4] = vine[4][1];
      leafPz[3][4] = vine[5][1];
      leafPz[4][4] = vine[6][1];


      temph += 3;
      if (i == grow-1) {
         vine[i][5] = 0;
      }
   }

   // Makes sure all values are correct

   for (int j = grow; j < 12; j++) {
      vine[j][1] = vine[j-1][1] + 0.1;
   }

   // grow vine
   complexCirc(grow,vine,11,texV);

   glPushMatrix();
   //glScaled(1 - time/100.0,1 - time/100.0,1 - time/100.0);

   //Add some good leaves if you need to
   for (int k = 0; k < 5; k++) {
      //scalelf = 0;
      if (lmode == 0) {
         leaf1(leafPz[k][0],leafPz[k][1],leafPz[k][2],leafPz[k][3],leafPz[k][4]/(1+ time/100.0),leafPz[k][5],leafPz[k][6]/2.0);
      }
      else {
         leaf2(leafPz[k][0],leafPz[k][1],leafPz[k][2],leafPz[k][3],leafPz[k][4],leafPz[k][5],leafPz[k][6]/2.0);
      }
      //scalelf = 1;
   }

   if (lmode == 0) {
      leaf1(0,0,0,0,8.0/(1+ time/100.0),0,4);
      leaf1(90,0,0,0,8.0/(1+ time/100.0),0,4);
      leaf1(180,0,0,0,8.0/(1+ time/100.0),0,4);
      leaf1(270,0,0,0,8.0/(1+ time/100.0),0,4);

      leaf1(45,0,0,0,8.0/(1+ time/100.0),0,2);
      leaf1(135,0,0,0,8.0/(1+ time/100.0),0,2);
      leaf1(225,0,0,0,8.0/(1+ time/100.0),0,2);
      leaf1(315,0,0,0,8.0/(1+ time/100.0),0,2);
   }
   else {
      leaf2(0,0,0,0,8.0,0,4);
      leaf2(90,0,0,0,8.0,0,4);
      leaf2(180,0,0,0,8.0,0,4);
      leaf2(270,0,0,0,8.0,0,4);

      leaf2(45,0,0,0,8.0,0,2);
      leaf2(135,0,0,0,8.0,0,2);
      leaf2(225,0,0,0,8.0,0,2);
      leaf2(315,0,0,0,8.0,0,2);
   }

   glPopMatrix();
   glPopMatrix();
}

// Funciton to make dying leaves
void deathLeaf(int az, int pt, int ro, float x, float y, float z, float l,float s) {
   glPushMatrix();

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[11]);

   float ts = time/100.00;
   float tc = (1/50.0)*time;

   glColor3f(0+tc,1,0);

   glTranslated(x,y,z);
   glRotated(az,0,1,0);
   glRotated(pt,1,0,0);
   glRotated(ro,0,0,1);
   glScaled(s,s,s);

   // Make the actual polygon

   glBegin(GL_POLYGON);

   glNormal3f(0,1*l,0.5*l);

   glTexCoord2f(0.5,2/6.0);
   glVertex3d(0,-0.5,-2);

   glTexCoord2f(2.5/6.0,0.5/6.0);
   glVertex3d(-0.5,0-ts/2.0,-0.5);

   glTexCoord2f(2/6.0,0.0);
   glVertex3d(-1,0,0);

   glTexCoord2f(1/6.0,0.0);
   glVertex3d(-2,0,0);

   glTexCoord2f(0,1/6.0);
   glVertex3d(-3,0,-1);

   glTexCoord2f(0,3/6.0);
   glVertex3d(-3,0+ts,-3);

   glTexCoord2f(1/6.0,4.5/6.0);
   glVertex3d(-2,0,-4.5);

   glTexCoord2f(0.5,1.0);
   glVertex3d(0,0-ts*5.0,-6-ts*2);

   glEnd();

   glBegin(GL_POLYGON);

   glTexCoord2f(0.5,2/6.0);
   glVertex3d(0,-0.5,-2);

   glTexCoord2f(3.5/6.0,0.5/6.0);
   glVertex3d(0.5,0,-0.5);

   glTexCoord2f(4.0/6.0,0.0);
   glVertex3d(1,0+ts,0);

   glTexCoord2f(5/6.0,0.0);
   glVertex3d(2,0,0);

   glTexCoord2f(1,1/6.0);
   glVertex3d(3,0-ts/2.0,-1);

   glTexCoord2f(1,3/6.0);
   glVertex3d(3,0,-3);

   glTexCoord2f(5/6.0,4.5/6.0);
   glVertex3d(2,0,-4.5);

   glTexCoord2f(0.5,1.0);
   glVertex3d(0,0-ts*5.0,-6-ts*2);

   glEnd();

   glDisable(GL_TEXTURE_2D);

   glPopMatrix();
}

// Draw the whole dying plant
void dplant() {
   glPushMatrix();

   float dstems[8][6] = {
      {0,0,0,0,0,0},
      {0,0,0,0,0,0.3},
      {0,4,0,0,0,0.2},
      {2,7,0,0,0,0.2},
      {8,10,0,0,0,0},
   };

   int texdstems[4] = {5,5,5,5};
   int az = 0;
   int pt = 0;

   // first ring
   for (int i = 0; i < 8; i++) {
      glPushMatrix();

      glRotated(az,0,1,0);
      glRotated(pt,1,0,0);
      complexCirc(5,dstems,4,texdstems);
      deathLeaf(180,0,0,0,10,6,-1,1);

      if (i % 2 == 0) {
         pt = 0;
      }
      else {
         //pt = -20;
      }
      az += 45;


      glPopMatrix();
   }

   az = 10;

   pt = -20;

   // second ring

   for (int i = 0; i < 8; i++) {
      glPushMatrix();

      glRotated(az,0,1,0);
      glRotated(pt,1,0,0);
      deathLeaf(180,0,0,0,8,7,-1,1);

      //pt += 3;
      az += 45;

      glPopMatrix();
   }

   az = 25;

   pt = 10;

   // Third ring

   for (int i = 0; i < 8; i++) {
      glPushMatrix();

      glRotated(az,0,1,0);
      glRotated(pt,1,0,0);
      deathLeaf(180,0,0,0,10,6,-1,1);

      //pt += 3;
      az += 45;


      glPopMatrix();
   }

   az = 25;

   pt = -30;

   for (int i = 0; i < 4; i++) {
      glPushMatrix();

      glRotated(az,0,1,0);
      glRotated(pt,1,0,0);
      deathLeaf(180,0,0,0,8,6,-1,1);

      //pt += 3;
      az += 90;

      glPopMatrix();
   }

   glPopMatrix();
}


/*
 *  OpenGL (GLUT) calls this routine to display the scene
 * modifed from ex9
 */
void display()
{

   // ortho and perspective projection code from ex9
   const double len=20.0;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (mode==1)
   {
      double Ex = -2*look*Sin(th)*Cos(ph);
      double Ey = +2*look        *Sin(ph);
      double Ez = +2*look*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }

   glTranslated(0,-10,0);

   // LIGHT STUFF ------------------------------------
   if (light == 1 | light == 2)
   {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        //  Light position
        float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        ball(Position[0],Position[1],Position[2] , 0.1);

        //GLfloat DiffuseLight[] = {1, 1, 0.5}; 
        //GLfloat AmbientLight[] = {0.5, 0.5, 0.1};

        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else if (light == 0) {
      glDisable(GL_LIGHTING);
   }
   // -----------------------------------------------------

   if (pmode == 1) { // make one of two pots
      complexCirc(8,bowl2,7,texB2);
   }
   else {
      complexCirc(8,bowl1,7,texB1);
   }

   if (tmode == 0) { // if type is zero make one large flower

      if (smode == 1) {
         stem1(5,stemPoints1,1,flowerPoints,18,leafPoints1,1);
      }
      else {
         stem2(5,stemPoints1,1,flowerPoints,20,leafPoints2,1);
      }

   }
   else if (tmode == 1) {
      vine();
   }
   else if (tmode == 2) {
      dplant();
   }
   else { // if type is 1 make a lot of flowers

      bamboo();

   }

   //leaf2(0,0,0,0,0,15,1);

   ground();
   
   //  Draw axes (from ex9)
   glColor3f(1,1,1);

   if (axes) // disabled by default
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Display UI and user choices
   char leafs[20];
   char stems[20];
   char types[20];
   char flowers[20];
   char lights[20];
   char pots[20];

   if (lmode == 0) {
      strcpy(leafs, "normal");
   }
   else {
      strcpy(leafs, "spikey");
   }

   if (smode == 1) {
      strcpy(stems, "one flower");
   }
   else {
      strcpy(stems, "multi flower");
   }

   if (tmode == 0) {
      strcpy(types, "flowering plant");
   }
   else if (tmode == 1) {
      strcpy(types, "vine");
      strcpy(flowers, "---------");
      if (smode == 0) {
         strcpy(stems, "sharper");
      }
      else {
         strcpy(stems, "smoother");
      }
   }
   else if (tmode == 2) {
      strcpy(types, "dying plant");
      strcpy(stems, "---------");
      strcpy(flowers, "---------");
      strcpy(leafs, "dying");
   }
   else {
      strcpy(types, "bamboo");
      strcpy(stems, "---------");
      strcpy(flowers, "---------");
   }

   if (fmode == 0) {
      strcpy(flowers, "connected");
   }
   else {
      strcpy(flowers, "star");
   }

   if (light == 1) {
      strcpy(lights, "day");
   }
   else if (light == 2) {
      strcpy(lights, "night");
   }
   else {
      strcpy(lights, "none");
   }

   if (pmode == 0) {
      strcpy(pots, "bowl");
   }
   else {
      strcpy(pots, "terracotta");
   }

   glWindowPos2i(5,30);
   Print("time: %d | light: %s",time,lights);
   //  Render the scene and make it visible

   glWindowPos2i(5,10);
   Print("pot: %s | leaf: %s | stem: %s | flower: %s | type: %s",pots,leafs,stems,flowers,types);
   glFlush();
   glutSwapBuffers();

   errCheck("display sanity check");
}

/*
 *  GLUT calls this routine when an arrow key is pressed 
 * from ex9
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT && mode != 2)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT && mode != 2)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP && mode != 2)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN && mode != 2)
      ph -= 5;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT key binds. some from ex9 most modified
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle if not in m 2
   else if (ch == '0' && mode != 2)
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'z' || ch == 'Z')
      axes = 1-axes;
   //  Switch display mode
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;

   else if (ch == 'l') { // change leaf
      lmode = 1 - lmode;
   }

   else if (ch == 's') { // change stem
      smode = 1 - smode;
   }

   else if (ch == 'p') { // change pot
      pmode = 1 - pmode;
   }

   else if (ch == 'f') { // change flower
      fmode = 1 - fmode;
   }

   else if (ch == 't') { // change type
      tmode += 1;
      tmode = tmode % 4;
   }

   else if (ch == ' ' && timeMove == 0){
      timea += 5;

      time = fabs(50*Sin(timea));
   }
   else if (ch == 'x'){
      timeMove = 1 - timeMove;
      time = 0;
   }

   else if (ch == '1') {
      light += 1;
      light = light %3;

      if (light == 2) {
         ambient = 20;
         emission = 70;
      }
      else if (light == 1) {
         ambient = 60;
         emission = 100;
      }
   }

   else if (ch == '[') {
      look -= 3;
   }

   else if (ch == ']') {
      look += 3;
   }

   if (look < 10) {
      look = 10;
   }

   if (look > 50) {
      look = 50;
   }

   if (time > mtime) {
      //time = 0;
   }

   if (time < 0) {
      //time = mtime;
   }

   //  Reproject
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

void idle()
{

   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   //th = fmod(90*t,360.0)/2.0;
   if (timeMove) {
      time = fabs(50*Sin(t*30));
   }
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized (from ex9)
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project();
}

/*
 *  Start up GLUT and tell it what to do (modifed from ex9)
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(1000,600);
   glutCreateWindow("final project - Percy Bell");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user

   // Do all the textures
   texture[0] = LoadTexBMP("flower.bmp");
   texture[1] = LoadTexBMP("bowl.bmp");
   texture[2] = LoadTexBMP("leaf.bmp");
   texture[3] = LoadTexBMP("pottop.bmp");
   texture[4] = LoadTexBMP("potbottom.bmp");
   texture[5] = LoadTexBMP("potBrown.bmp");
   texture[6] = LoadTexBMP("bamboo.bmp");
   texture[7] = LoadTexBMP("leaf2.bmp");
   texture[8] = LoadTexBMP("tile.bmp");
   texture[9] = LoadTexBMP("fwall.bmp");
   texture[10] = LoadTexBMP("vine1.bmp");
   texture[11] = LoadTexBMP("dleaf.bmp");

   glutIdleFunc(idle);
   glutMainLoop();
   return 0;
}
