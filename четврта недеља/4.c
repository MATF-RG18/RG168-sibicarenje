#include <GL/glut.h>
#include <math.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 

#define TIMER_INTERVAL 20
#define ESC 27
#define PI (3.1415926535897932384626433)

// dimenzije prozora
static int window_width, window_height;

// pocetne koordinate pozicije kamere
int X = 7;
int Y = 0;
int Z = 7;
//promenljiva koja odredjuje da li je animacija u toku
int animacija = 0;

//odredjuje da li je pokrenuta igra kako bi sprecila visestruko
//pritiskanje tastera g
int start = 0;

//odredjuje da li igrac i dalje ima pravo da pokrece igru
int status = 1;

//duzina stola
const double duzina = 3.0;
//ugao sluzi za pocetnu animaciju gde casa poklapa lopticu, ugao0 za rotacije
double ugao = 0, ugao0 = 0;
//akc odredjuje koja ce se akcija preduzeti, tj kako ce se case kretati, dodatno objasnjenje u funkciji sto()
int akc = -1;

//promenljiva koja odredjuje u kojoj casi je kuglica, na pocetku je kuglica u drugoj casi, 1 prva, 2 druga, 3 treca
int kuglica = 2;
//koordinate kuglice
double kuglica_x = 0.0, kuglica_y = 0.0;

//odredjuje da li je zavrseno rotiranje casa tj da li je dozvoljeno da se pogadja casa u kojoj je loptica
int zavrseno = 0;
//1 igrac odabrao prvu casu, 2 odabrao drugu, 3 odabrao trecu
int odabrana_casa = 0;

//izdvojena funkcija zbog cestog koriscenja istog koda u razlicitim funkcijama
void materijal_kuglice(){
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
}

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

//ako je promaseno gde se kuglica nalazi, ova funkcija ce podici casu u kojoj se nalazi kuglica
void otkrij_casu_zbor_promasaja(){
    //vec smo odabrali casu koja je pogresna, sada gledamo gde je kuglica
    
    glPushMatrix();
    //ako je kuglica u prvoj casi, treba da prvu casu podignemo
    if(kuglica == 1){
        if(ugao0 > PI/2){
            //radi korekcije
            glTranslated(-2, -duzina, 0);
            glRotatef(270, 0, 1, 0);
        }
        else{
            //vrsi se animacija unazad (u odnosu na onu pocetnu)
            glTranslated(-2*sin(ugao0), -duzina, sin(2*ugao0));
            glRotatef(0-(sin(ugao0))*90, 0, 1, 0);
        }
    }
    else{
        //ako kuglica nije u prvoj casi, a mi smo odabrali prvu casu, ona treba da ostane otkrivena
        if(odabrana_casa == 1){
            glTranslated(-2, -duzina, 0);
            glRotatef(270, 0, 1, 0);
        }
        else
            //u suprotnom, casu pozicioniramo na svoje mesto tako da bude neotkrivena
            glTranslated(0, -duzina, 0);
    }
    napravi_casu();
    glPopMatrix();
    
    
    //isto kao malopre samo ispitujemo da li je kuglica u drugoj casi
    glPushMatrix();
    if(kuglica == 2){
        if(ugao0 > PI/2){
            glTranslated(-2, 0, 0);
            glRotatef(270, 0, 1, 0);
        }
        else{
            glTranslated(-2*sin(ugao0), 0, sin(2*ugao0));
            glRotatef(0-(sin(ugao0))*90, 0, 1, 0);
        }
    }
    else{
        if(odabrana_casa == 2){
            glTranslated(-2, 0, 0);
            glRotatef(270, 0, 1, 0);
        }
        else
            glTranslated(0, 0, 0);
    }
        
    napravi_casu();
    glPopMatrix();
    
    //isto kao malopre samo ispitujemo da li je kuglica u trecoj casi
    glPushMatrix();
    if(kuglica == 3){
        if(ugao0 > PI/2){
            glTranslated(-2, duzina, 0);
            glRotatef(270, 0, 1, 0);
        }
        else{
            glTranslated(-2*sin(ugao0), duzina, sin(2*ugao0));
            glRotatef(0-(sin(ugao0))*90, 0, 1, 0);
        }
    }
    else{
        if(odabrana_casa == 3){
            glTranslated(-2, duzina, 0);
            glRotatef(270, 0, 1, 0);
        }
        else
            glTranslated(0, duzina, 0);
    }
        
    napravi_casu();
    glPopMatrix();
   
    //pravimo kuglicu
    materijal_kuglice();
    glPushMatrix();
    glTranslated(kuglica_x, kuglica_y, -0.5);
    glutSolidSphere(0.2, 30, 30);
    glPopMatrix();
    
}

void otkrij_casu(){
    
    glPushMatrix();
    if(odabrana_casa == 1){
        if(ugao > PI/2){
            //radi korekcije
            glTranslated(-2, -duzina, 0);
            glRotatef(270, 0, 1, 0);
        }
        else{
            //radi se animacija "otkrivanja" case, ali sada unazad
            glTranslated(-2*sin(ugao), -duzina, sin(2*ugao));
            glRotatef(0-(sin(ugao))*90, 0, 1, 0);
        }
    }
    else
        //ako nije odabrana prva casa casa se samo postavlja na odgovarajuce mesto
        glTranslated(0, -duzina, 0);
    napravi_casu();
    glPopMatrix();
    
    //isto za drugu casu
    glPushMatrix();
    if(odabrana_casa == 2){
        if(ugao > PI/2){
            glTranslated(-2, 0, 0);
            glRotatef(270, 0, 1, 0);
        }
        else{
            glTranslated(-2*sin(ugao), 0, sin(2*ugao));
            glRotatef(0-(sin(ugao))*90, 0, 1, 0);
        }
    }
    else
        glTranslated(0, 0, 0);
    napravi_casu();
    glPopMatrix();
    
    //isto za trecu casu
    glPushMatrix();
    if(odabrana_casa == 3){
        if(ugao > PI/2){
            glTranslated(-2, duzina, 0);
            glRotatef(270, 0, 1, 0);
        }
        else{
            glTranslated(-2*sin(ugao), duzina, sin(2*ugao));
            glRotatef(0-(sin(ugao))*90, 0, 1, 0);
        }
    }
    else
        glTranslated(0, duzina, 0);
    napravi_casu();
    glPopMatrix();
   
    //pravimo kuglicu
    materijal_kuglice();
    glPushMatrix();
    glTranslated(kuglica_x, kuglica_y, -0.5);
    glutSolidSphere(0.2, 30, 30);
    glPopMatrix();
}

//funkcija koja pravi sve case i kuglicu
void _case(){
    
    //pravimo prvu casu (levu)
    glPushMatrix();
    if (kuglica == 1){
        if(ugao > PI/2){
            //radi korekcije, posto pi ne moze da bude precizno ne moze ni translacija da bude tacna u potpunosti pa ce casa biti blago, skoro neprimetno pomerena, ali da bi case stajale tacno u liniji postavlja se ovako
            glTranslated(0, -duzina, 0);
            glRotatef(0, 0, 1, 0);
        }
        else{
            //radi se animacija poklapanja kuglice
            glTranslated(-2*cos(ugao), -duzina, sin(2*ugao));
            glRotatef(270+(sin(ugao))*90, 0, 1, 0);
        }
    }
    else
        //ako kuglica nije u prvoj casi samo pozicioniramo casu
        glTranslated(0, -duzina, 0);
    napravi_casu();
    glPopMatrix();
    
    //pravimo drugu casu, ova je oborena na samom pocetku, ista prica kao iznad
    glPushMatrix();
    if (kuglica == 2){
        if(ugao > PI/2){
            glTranslated(0, 0, 0);
            glRotatef(0, 0, 1, 0);
        }
        else{
            glTranslated(-2*cos(ugao), 0, sin(2*ugao));
            glRotatef(270+(sin(ugao))*90, 0, 1, 0);
        }
    }
    else
        glTranslated(0, 0, 0);
    napravi_casu();
    glPopMatrix();

    //pravimo trecu
    glPushMatrix();
    if (kuglica == 3){
        if(ugao > PI/2){
            glTranslated(0, duzina, 0);
            glRotatef(0, 0, 1, 0);
        }
        else{
            glTranslated(-2*cos(ugao), duzina, sin(2*ugao));
            glRotatef(270+(sin(ugao))*90, 0, 1, 0);
        }
    }
    else
        glTranslated(0, duzina, 0);
    napravi_casu();
    glPopMatrix();
    
    //pravimo kuglicu
    materijal_kuglice();
    glPushMatrix();
    glTranslated(kuglica_x, kuglica_y, -0.5);
    glutSolidSphere(0.2, 30, 30);
    glPopMatrix();
}

//azuriranja kuglica se rade po sledecoj tabeli (izracunato na osnovu odgovarajuce akcije i toga gde se kuglica trenutno nalazi
// akc              kuglica             nova_pozicija_kuglice
// 0                1                   2                       // 1 2 3 -> 2 1 3 (rotiranje 1. i 2. case)
// 0                2                   1
// 0                3                   3
// 1                1                   1                       // 1 2 3 -> 1 3 2
// 1                2                   3
// 1                3                   2
// 2                1                   3                       // 1 2 3 -> 3 2 1
// 2                2                   2
// 2                3                   1
// 3                1                   2                       // 1 2 3 -> 3 1 2
// 3                2                   3
// 3                3                   1
// 4                1                   3                       // 1 2 3 -> 2 3 1
// 4                2                   1
// 4                3                   2

void _case0(){
    
    if(ugao0 > PI/2){
        //rotacija je zavrsena
        if(kuglica == 1){
            //kuglica je bila je na mestu 1 (u prvoj casi), treba da se azurira da je na mestu 2
            kuglica = 2;
            //azuriramo y koordinatu kuglice (x je uvek 0)
            kuglica_y = 0;
        }
        else if(kuglica == 2){
            //kuglica je bila je na mestu 2, treba da se azurira da je na mestu 1
            kuglica = 1;
            kuglica_y = -duzina;
        }
        //jos jedan slucaj bi bio da je kuglica bila u trecoj casi, medjutim, tada ona ostaje u trecoj
        //pozivamo funkciju _case() koja ce da iscrta pocetnu postavku (gde su sve case u liniji i okrenute normalno)
        _case();
        return;
    }
    
    //animacija traje, iscrtavamo case sa odgovarajucim uglovima
    //casa1
    glPushMatrix();
    glTranslated(-sin(2*ugao0), -duzina+sin(ugao0)*duzina, 0);
    napravi_casu();
    glPopMatrix();
    
    //casa2
    glPushMatrix();
    glTranslated(sin(2*ugao0), -sin(ugao0)*duzina, 0);
    napravi_casu();
    glPopMatrix();
     
    //casa3
    glPushMatrix();
    glTranslated(0, duzina, 0);
    napravi_casu();
    glPopMatrix();
    
    //pravimo kuglicu
    
    materijal_kuglice();
    
    //kuglica prati rotaciju one case u kojoj se nalazi
    if(kuglica == 1){
        glPushMatrix();
        glTranslated(kuglica_x-sin(2*ugao0), kuglica_y+sin(ugao0)*duzina, -0.5);
        glutSolidSphere(0.2, 30, 30);
        glPopMatrix();
    }
    else if(kuglica == 2){
        glPushMatrix();
        glTranslated(kuglica_x+sin(2*ugao0), kuglica_y-sin(ugao0)*duzina, -0.5);
        glutSolidSphere(0.2, 30, 30);
        glPopMatrix();
    }
    else{
        glPushMatrix();
        glTranslated(kuglica_x, kuglica_y, -0.5);
        glutSolidSphere(0.2, 30, 30);
        glPopMatrix();
    }
}

void _case1(){
    
    if(ugao0 > PI/2){
        //rotacija je zavrsena
        if(kuglica == 2){
            //bila je na mestu 2, treba da se azurira da je na mestu 3
            kuglica = 3;
            //azuriramo y koordinatu kuglice (x je uvek 0)
            kuglica_y = duzina;
        }
        else if(kuglica == 3){
            //kuglica je bila je na mestu 3, treba da se azurira da je na mestu 2
            kuglica = 2;
            kuglica_y = 0;
        }
        //jos jedan slucaj bi bio da je kuglica bila u prvoj casi, medjutim, tada ona ostaje u prvoj
        _case();
        return;
    }
    
    //animacija traje, iscrtavamo case sa odgovarajucim uglovima
    //casa1
    glPushMatrix();
    glTranslated(0, -duzina, 0);
    napravi_casu();
    glPopMatrix();
    
    //casa2
    glPushMatrix();
    glTranslated(-sin(2*ugao0), sin(ugao0)*duzina, 0);
    napravi_casu();
    glPopMatrix();
    
    //casa3
    glPushMatrix();
    glTranslated(sin(2*ugao0), duzina-sin(ugao0)*duzina, 0);
    napravi_casu();
    glPopMatrix();
    
    //pravimo kuglicu
    materijal_kuglice();
    
    //kuglica prati rotaciju one case u kojoj se nalazi
    if(kuglica == 2){
        glPushMatrix();
        glTranslated(kuglica_x-sin(2*ugao0), kuglica_y+sin(ugao0)*duzina, -0.5);
        glutSolidSphere(0.2, 30, 30);
        glPopMatrix();
    }
    else if(kuglica == 3){
        glPushMatrix();
        glTranslated(kuglica_x+sin(2*ugao0), kuglica_y-sin(ugao0)*duzina, -0.5);
        glutSolidSphere(0.2, 30, 30);
        glPopMatrix();
    }
    else{
        glPushMatrix();
        glTranslated(kuglica_x, kuglica_y, -0.5);
        glutSolidSphere(0.2, 30, 30);
        glPopMatrix();
    }
    
    
}

void _case2(){
    
    if(ugao0 > PI/2){
        //rotacija je zavrsena
        if(kuglica == 3){
            //bila je na mestu 3, treba da se azurira da je na mestu 1
            kuglica = 1;
            //azuriramo y koordinatu kuglice (x je uvek 0)
            kuglica_y = -duzina;
        }
        else if(kuglica == 1){
            //bila je na mestu 1, treba da se azurira da je na mestu 3
            kuglica = 3;
            kuglica_y = duzina;
        }
        //jos jedan slucaj bi bio da je kuglica bila u drugoj casi, medjutim, tada ona ostaje u drugoj
        _case();
        return;
    }
    
    //animacija traje, iscrtavamo case sa odgovarajucim uglovima
    //casa1
    glPushMatrix();
    glTranslated(-2*sin(2*ugao0), -duzina+sin(ugao0)*2*duzina, 0);
    napravi_casu();
    glPopMatrix();
    
    //casa2
    glPushMatrix();
    napravi_casu();
    glPopMatrix();
    
    //casa3
    glPushMatrix();
    glTranslated(2*sin(2*ugao0), duzina-sin(ugao0)*2*duzina, 0);
    napravi_casu();
    glPopMatrix();
    
    //pravimo kuglicu
    materijal_kuglice();
    
    //kuglica prati rotaciju one case u kojoj se nalazi
    if(kuglica == 1){
        glPushMatrix();
        glTranslated(kuglica_x-2*sin(2*ugao0), kuglica_y+sin(ugao0)*2*duzina, -0.5);
        glutSolidSphere(0.2, 30, 30);
        glPopMatrix();
    }
    else if(kuglica == 3){
        glPushMatrix();
        glTranslated(kuglica_x+2*sin(2*ugao0), kuglica_y-sin(ugao0)*2*duzina, -0.5);
        glutSolidSphere(0.2, 30, 30);
        glPopMatrix();
    }
    else{
        glPushMatrix();
        glTranslated(kuglica_x, kuglica_y, -0.5);
        glutSolidSphere(0.2, 30, 30);
        glPopMatrix();
    }
    
}

void _case3(){
    
    if(ugao0 > PI/2){
        //rotacija je zavrsena
        if(kuglica == 1){
            //bila je na mestu 1, treba da se azurira da je na mestu 2
            kuglica = 2;
            //azuriramo y koordinatu kuglice (x je uvek 0)
            kuglica_y = 0;
        }
        else if(kuglica == 2){
            //bila je na mestu 2, treba da se azurira da je na mestu 3
            kuglica = 3;
            kuglica_y = duzina;
        }
        
        else{
            //bila je na mestu 3, treba da se azurira da je na mestu 1
            kuglica = 1;
            kuglica_y = -duzina;
        }
        _case();
        return;
    }
    
    //animacija traje, iscrtavamo case sa odgovarajucim uglovima
    //casa1
    glPushMatrix();
    glTranslated(-sin(2*ugao0), -duzina+sin(ugao0)*duzina, 0);
    napravi_casu();
    glPopMatrix();
    
    //casa2
    glPushMatrix();
    glTranslated(-sin(2*ugao0), sin(ugao0)*duzina, 0);
    napravi_casu();
    glPopMatrix();
    
    //casa3
    glPushMatrix();
    //sin5/2x i sin2x imaju presek u 2/9pi
    //odradjeno kako se case ne bi dodirivale
    if(ugao0 < 2*PI/9)
        glTranslated(sin(2.5*ugao0), duzina-sin(ugao0)*2*duzina, 0);
    else
        glTranslated(sin(2*ugao0), duzina-sin(ugao0)*2*duzina, 0);
    napravi_casu();
    glPopMatrix();
    
    //pravimo kuglicu
    materijal_kuglice();
    //kuglica prati rotaciju one case u kojoj se nalazi
    if(kuglica == 1){
        glPushMatrix();
        glTranslated(kuglica_x-sin(2*ugao0), kuglica_y+sin(ugao0)*duzina, -0.5);
        glutSolidSphere(0.2, 30, 30);
        glPopMatrix();
    }
    else if(kuglica == 2){
        glPushMatrix();
        glTranslated(kuglica_x-sin(2*ugao0), kuglica_y+sin(ugao0)*duzina, -0.5);
        glutSolidSphere(0.2, 30, 30);
        glPopMatrix();
    }
    else{
        glPushMatrix();
        
        if(ugao0 < 2*PI/9)
            glTranslated(kuglica_x+sin(2.5*ugao0), kuglica_y-sin(ugao0)*2*duzina, 0);
        else
            glTranslated(kuglica_x+sin(2*ugao0), kuglica_y-sin(ugao0)*2*duzina, 0);
        
        glutSolidSphere(0.2, 30, 30);
        glPopMatrix();
    }
}

void _case4(){
    
    if(ugao0 > PI/2){
        //rotacija je zavrsena
        if(kuglica == 1){
            //bila je na mestu 1, treba da se azurira da je na mestu 3
            kuglica = 3;
            //azuriramo y koordinatu kuglice (x je uvek 0)
            kuglica_y = duzina;
        }
        else if(kuglica == 2){
            //bila je na mestu 2, treba da se azurira da je na mestu 1
            kuglica = 1;
            kuglica_y = -duzina;
        }
        
        else{
            //bila je na mestu 3, treba da se azurira da je na mestu 2
            kuglica = 2;
            kuglica_y = 0;
        }
        _case();
        return;
    }
    
    //animacija traje, iscrtavamo case sa odgovarajucim uglovima
    //casa1
    glPushMatrix();
    //sin5/2x i sin2x imaju presek u 2/9pi
    //odradjeno kako se case ne bi dodirivale
    if(ugao0 < 2*PI/9)
        glTranslated(-sin(2.5*ugao0), -duzina+sin(ugao0)*2*duzina, 0);
    else
        glTranslated(-sin(2*ugao0), -duzina+sin(ugao0)*2*duzina, 0);
    napravi_casu();
    glPopMatrix();
    
    //casa2
    glPushMatrix();
    glTranslated(sin(2*ugao0), -sin(ugao0)*duzina, 0);
    napravi_casu();
    glPopMatrix();
    
    //casa3
    glPushMatrix();
    glTranslated(sin(2*ugao0), duzina-sin(ugao0)*duzina, 0);
    napravi_casu();
    glPopMatrix();
    
    //pravimo kuglicu
    materijal_kuglice();
    //kuglica prati rotaciju one case u kojoj se nalazi
    if(kuglica == 1){
        glPushMatrix();
        if(ugao0 < 2*PI/9)
            glTranslated(kuglica_x-sin(2.5*ugao0), kuglica_y+sin(ugao0)*2*duzina, 0);
        else
            glTranslated(kuglica_x-sin(2*ugao0), kuglica_y+sin(ugao0)*2*duzina, 0);
        glutSolidSphere(0.2, 30, 30);
        glPopMatrix();
    }
    else if(kuglica == 2){
        glPushMatrix();
        glTranslated(kuglica_x+sin(2*ugao0), kuglica_y-sin(ugao0)*duzina, 0);
        glutSolidSphere(0.2, 30, 30);
        glPopMatrix();
    }
    else{
        glPushMatrix();
        glTranslated(kuglica_x+sin(2*ugao0), kuglica_y-sin(ugao0)*duzina, 0);
        glutSolidSphere(0.2, 30, 30);
        glPopMatrix();
    }
    
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
    
    //u zavisnosti od vrednosti akcije radimo promene na stolu
    //0 zameni prvu i drugu casu 1 2 3 -> 2 1 3
    //1 drugu i trecu            1 2 3 -> 1 3 2
    //2 trecu i prvu             1 2 3 -> 3 2 1
    //3 prve dve i trecu         1 2 3 -> 3 1 2
    //4 druge dve i prvu         1 2 3 -> 2 3 1
    //-1 je pocetna, default vrednost
    //dodaju se jos dve akcije
    //5 otkrij casu koju je izabrao korisnik
    //6 ako je pogresno odabrao, otkrij onu casu u kojoj se nalazi kuglica
    
    switch(akc){
        case 0:
            _case0();
             break;
        case 1:
            _case1();
             break;
        case 2:
            _case2();
             break;
        case 3:
            _case3();
             break;
        case 4:
            _case4();
             break;
        case 5:
             otkrij_casu();
             break;
        case 6:
             otkrij_casu_zbor_promasaja();
             break;
        default: //pocetni slucaj
            _case();
    }
    
    glPopMatrix();
}

//tajmer funkcija koja se poziva kako bi odradila animaciju podizanja
//case u kojoj se nalazi kuglica nakon sto je usledio promasaj
static void on_timer4(int value){
    
    // povecava se ugao
    ugao0 += 0.1;

    // forsira se ponovno iscrtavanje prozora
    glutPostRedisplay();
    
    //pozivaj tajmer funkciju sve dok ugao0 ne postane veci od pi/2
    if(ugao0 > PI/2)
        ugao0 = PI/2;
    else
        glutTimerFunc(TIMER_INTERVAL, on_timer4, 0);
}

//tajmer funkcija koja se poziva da bi se otkrila casa koju
//je korisnik odabrao
static void on_timer3(int value){

    // povecava se ugao
    ugao += 0.1;

    // forsira se ponovno iscrtavanje prozora
    glutPostRedisplay();
    
    //slicno kao u funkciji on_timer
    if(ugao > PI/2)
        animacija = 0, ugao = PI/2+0.000001;
    // ponovo pozivamo tajmer funkciju ako je ispunjen uslov
    if (animacija) {
        glutTimerFunc(TIMER_INTERVAL, on_timer3, 0);
    }
    else{
        //ako uslov nije ispunjen znaci da treba da prijavimo da li je korisnik pogodio gde se nalazi kuglica
        if(kuglica != odabrana_casa){
            status = 0;
            akc = 6;
            glutTimerFunc(500, on_timer4, 0);
        }
    }
    
}

static void on_timer2(int value)
{
    //zelimo da se case 5 puta rotiraju
    if (value == 5){
        animacija = 0;
        //zavrsena je rotacija casa, tek je sada moguce da se pogadja u kojoj casi je kuglica
        zavrseno = 1;
        return;
    }
    
    //ako je start jednak 0 znaci da je pritisnut taster 'r' (restartovani su svi podaci) tj da treba prekinuti dalju animaciju
    if(start == 0)
        return;
    
    if (animacija == 0){
        //zavrsena je jedna rotacija, biramo narednu preko pseudoslucajnog broja
        akc = rand()%5;
        ugao0 = 0;
        animacija = 1;
        glutTimerFunc(TIMER_INTERVAL*5, on_timer2, value+1);
    }
    else{
        ugao0 += 0.05;
        glutPostRedisplay();
        if(ugao0 > PI/2)
            animacija = 0;        
        glutTimerFunc(TIMER_INTERVAL, on_timer2, value);
    }
    
}

static void on_timer(int value)
{
    // povecava se ugao
    ugao += 0.1;

    // forsira se ponovno iscrtavanje prozora
    glutPostRedisplay();
    
    //animaciju radimo tako sto ugao krece od 0 i ide do pi/2 (sin dakle od 0 do 1, cos obratno)
    //ako je ugao presao pi/2 prekidamo pocetnu animaciju i zapocinjemo rotiranje casa
    if(ugao > PI/2)
        animacija = 0, ugao = PI/2+0.000001; //postavljamo ugao na ovu vrednost jer je nesto veca od PI/2 a tada funkcija _case() postavlja tri case u normalan polozaj sto ce nam biti potrebno za animaciju kasnije
    // ponovo pozivamo tajmer funkciju ako je ispunjen uslov
    if (animacija) {
        glutTimerFunc(TIMER_INTERVAL, on_timer, 0);
    }
    else{
        //ako nije znaci da je pocetna animacija gde casa pokriva kuglicu zavrsena, pozivamo tajmer funkciju za rotacije
        glutPostRedisplay();
        glutTimerFunc(500, on_timer2, -1);
    }
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
        
        case '1':
        case '2':
        case '3':
            if(zavrseno == 1){
                //nije vise dozvoljeno da se bira opcija
                zavrseno = 0;
                akc = 5;
                ugao = 0;
                animacija = 1;
                odabrana_casa = key - '0';
                glutTimerFunc(TIMER_INTERVAL, on_timer3, 0);
            }
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
        
        //pritiskom na taster 'g' pocinje igra
        case 'g':
            //status ce spreciti nastavak animacije ako je pogresno izabrana casa,
            //a u donjoj disjunkciji promenljiva odabrana_casa ce sprecavati da se pritisne
            //g vise puta dok animacija jos traje, a start omogucava inicijalno pokretanje
            if ( ((animacija == 0 && odabrana_casa != 0) || start == 0) && status == 1) {
                akc = -1;
                start = 1;
                odabrana_casa = 0;
                ugao = 0;
                animacija = 1;
                glutTimerFunc(TIMER_INTERVAL, on_timer, 0);
            }
            break;
            
         //pritiskom na taster 'r' resetuju se svi podaci
         case 'r':
            animacija = 0;
            start = 0;
            status = 1;
            ugao = 0;
            ugao0 = 0;
            akc = -1;
            kuglica = 2;
            kuglica_x = 0.0, kuglica_y = 0.0;
            zavrseno = 0;
            odabrana_casa = 0;
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
    
    //postavljamo sito za funkciju srand
    srand(time(0));
    
    // ulazak u glavnu petlju
    glutMainLoop();
    
    return 0;
    
}
