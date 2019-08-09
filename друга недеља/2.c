#include <GL/glut.h>
#include <math.h>

#define ESC 27
#define PI (3.1415926535897932384626433)


// dimenzije prozora
static int window_width, window_height;

// pocetne koordinate pozicije kamere
int X = 10;
int Y = 0;
int Z = 3;

//duzina stola
const double duzina = 3.0;


//pomocna funkcija koja pravi jednu casu
void napravi_casu(){
    
    //casu cemo napraviti od cilindra i kruga
    glPushMatrix();
    
    glRotatef(180,1,0,0);
    glTranslated(0, 0, -0.6);
    GLUquadricObj* quadratic = gluNewQuadric();
    gluCylinder(quadratic, 0.35, 0.6, 1.3, 32, 32);
    
    glPushMatrix();
    //poluprecnik kruga je malo veci od prvog poluprecnika cilindra cisto radi korekcije buduci da se sa istim poluprecnikom pojavljuju "rupe"
    double r = 0.36;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0);
    int i;
    for (i = 0; i <= 20; i++)
        glVertex2f ( r * cos(i * 2.0 * PI / 20), r * sin(i * 2.0 *PI / 20) );
    glEnd(); 
    glPopMatrix();
    
    glPopMatrix();
}

//funkcija koja pravi sve case i kuglicu
void _case(){
    
    //pravimo prvu casu (levu)
    glPushMatrix();
    glTranslated(0, -duzina, 0);
    napravi_casu();
    glPopMatrix();
    
    //pravimo drugu casu, ova je oborena na pocetku posto ce odatle pocinjati animacija
    glPushMatrix();
    glTranslated(-2, 0, 0);
    glRotatef(270, 0, 1, 0);
    napravi_casu();
    glPopMatrix();

    //pravimo trecu
    glPushMatrix();
    glTranslated(0, duzina, 0);
    napravi_casu();
    glPopMatrix();
    
    //materijal kuglice
    GLfloat ambijent_materijala_kuglice[] = { 0.1, 0.1, 0.1, 1 };
    GLfloat difuznost_materijala_kuglice[] = { 0.1, 0.1, 0.1, 1 };
    GLfloat spekularnost_materijala_kuglice[] = { 1, 1, 1, 1 };    
    GLfloat osvetljenje_kuglice = 20;
    
    //postavljanje materijala kuglice
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambijent_materijala_kuglice);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, difuznost_materijala_kuglice);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spekularnost_materijala_kuglice);
    glMaterialf(GL_FRONT, GL_SHININESS, osvetljenje_kuglice);
    
    //pravimo kuglicu
    glPushMatrix();
    glTranslated(0, 0, -0.5);
    glutSolidSphere(0.2, 30, 30);
    glPopMatrix();
}

//funkcija koja pravi sto
void sto(){
    
    //materijal stola
    GLfloat ambijent_stola[] = { 0.3, 0.3, 0.5, 1 };
    GLfloat difuznost_stola[] = { 0.01, 0.06, 0.3, 1 };
    GLfloat spekularnost_stola[] = { 0.01, 0.06, 0.3, 1 };
    GLfloat osvetljenje_stola = 0.2;
    
    //postavljanje materijala stola
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambijent_stola);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, difuznost_stola);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spekularnost_stola);
    glMaterialf(GL_FRONT, GL_SHININESS, osvetljenje_stola);
    
    //pocinjemo pravljenje stola
    glPushMatrix();
    glTranslated(0,0,duzina/2);
    //pravi se ploca stola
    glPushMatrix();
    glScaled(duzina, duzina, 0.05*duzina);
    glTranslated(0,0,-2*duzina);
    glutSolidCube(duzina);
    glPopMatrix();
    
    //prave se nogare, razlika je samo u translaciji
    glPushMatrix();
    glTranslated(-duzina,duzina,-duzina-0.4);
    glScaled(0.05*duzina, 0.05*duzina, duzina/2);
    glutSolidCube(duzina);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(-duzina,-duzina,-duzina-0.4);
    glScaled(0.05*duzina, 0.05*duzina, duzina/2);
    glutSolidCube(duzina);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(duzina,-duzina,-duzina-0.4);
    glScaled(0.05*duzina, 0.05*duzina, duzina/2);
    glutSolidCube(duzina);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(duzina,duzina,-duzina-0.4);
    glScaled(0.05*duzina, 0.05*duzina, duzina/2);
    glutSolidCube(duzina);
    glPopMatrix();

    //materijal casa
    GLfloat ambijent_casa[] = { 1, 0, 0, 1 };
    GLfloat difuznost_casa[] = { 0.75, 0.05, 0.05, 1 };
    GLfloat spekularnost_casa[] = { 0.75, 0.05, 0.05, 1 };    
    GLfloat osvetljenje_casa = 10;
    
    //postavljanje materijala casa
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambijent_casa);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, difuznost_casa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spekularnost_casa);
    glMaterialf(GL_FRONT, GL_SHININESS, osvetljenje_casa);
    
    //pozivamo funkciju koja ce da nam napravi case
    _case();
    
    glPopMatrix();
}

static void on_display(void){

    // brise se prethodni sadrzaj prozora
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // podesava se viewport
    glViewport(0, 0, window_width, window_height);
    
    // podesava se projekcija
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, window_width/(float)window_height, 1, 50);
    
    // podesava se vidna tacka
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(X, Y, Z, 0, 0, 0, 0, 0, 1);
    
    // omogucava se upotreba svetlosti i prvog svetla
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // podesavanja svetla
    GLfloat pozicija_svetla[] = { 5, 6, 12, 1 };
    GLfloat ambijent_svetla[] = { 0.1, 0.0, 0.0, 1 };
    GLfloat difuznost_svetla[] = { 0.5,0.25,0.1, 1 };
    GLfloat spekularnost_svetla[] = { 2.2, 1.5, 1, 1 }; 
    
    // postavlja se svetlo
    glLightfv(GL_LIGHT0, GL_POSITION, pozicija_svetla);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambijent_svetla);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, difuznost_svetla);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spekularnost_svetla);
    
    //pravi se sto
    sto();
    
    // nova slika se salje na ekran
    glutSwapBuffers();
}

static void on_keyboard(unsigned char key, int x, int y){
    
    switch(key){
    
        // pritiskom na taster 'Esc' program se zavrsava
        case ESC:
            exit(0);
            break;
            
        //pritiskom na taster 'y' menja se pozicija kamere    
        case 'y':
            X = 1;
            Z = 10;
            glutPostRedisplay();
            break;
        
        //pritiskom na taster 'h' menja se pozicija kamere    
        case 'h':
            X = 7;
            Z = 7;
            glutPostRedisplay();
            break;
        
        //pritiskom na taster 'n' menja se pozicija kamere    
        case 'n':
            X = 10;
            Z = 3;
            glutPostRedisplay();
            break;
    }
}

static void on_reshape(int width, int height){
    
    // cuvaju se sirina i visina prozora
    window_width = width;
    window_height = height;
}

int main(int argc, char** argv){
    
    // inicijalizuje se GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    
    // kreira se prozor
    glutInitWindowSize(700, 500);
    glutInitWindowPosition(300, 300);
    glutCreateWindow("Sibicarenje");
    
    // registrovanje callback funkcija
    glutDisplayFunc(on_display);
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    
    // obavlja se OpenGL inicijalizacija
    glClearColor(0.5, 0.5, 0.5, 0);
    glEnable(GL_DEPTH_TEST);
    
    // ulazak u glavnu petlju
    glutMainLoop();
    
    return 0;
    
}
