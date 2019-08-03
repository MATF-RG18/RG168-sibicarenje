#include <GL/glut.h>

#define ESC 27

// dimenzije prozora
static int window_width, window_height;

// Pocetne koordinate pozicije kamere
int X = 10;
int Y = 0;
int Z = 3;

//duzina stola
const double duzina = 3.0;

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
    
    // inicijalizuje se GLUT-a
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
    
    // Ulazak u glavnu petlju
    glutMainLoop();
    
    return 0;
    
}
