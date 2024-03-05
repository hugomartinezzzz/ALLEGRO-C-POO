#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
const int SCREEN_WIDTH = 887;
const int SCREEN_HEIGHT = 885;
int partidas = 0;
struct Character {
    float x, y;
    float speed;
    ALLEGRO_BITMAP* image;
    float width, height;
    float collisionBoxX, collisionBoxY;
    float collisionBoxWidth, collisionBoxHeight;
};

void must_init(bool test, const char* description)
{
    if (test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

void drawCharacter(Character& character) {
    al_draw_bitmap(character.image, character.x, character.y, 0);
}

void drawBackground(ALLEGRO_BITMAP* background) {
    al_draw_bitmap(background, 0, 0, 0);
}

void moveTowards(Character& from, const Character& to) {
    float angle = atan2(to.y - from.y, to.x - from.x);
    from.x += from.speed * cos(angle);
    //from.y += from.speed * sin(angle);

    from.collisionBoxX = from.x;
    from.collisionBoxY = from.y;
}
bool checkCollision(const Character& a, const Character& b) {
    // Verifica si las cajas de colisión de a y b se superponen
    cout << "a: (" << a.collisionBoxX << ", " << a.collisionBoxY << "), w: " << a.collisionBoxWidth << ", h: " << a.collisionBoxHeight << endl;
    cout << "b: (" << b.collisionBoxX << ", " << b.collisionBoxY << "), w: " << b.collisionBoxWidth << ", h: " << b.collisionBoxHeight << endl;
     return (a.collisionBoxX < b.collisionBoxX + b.collisionBoxWidth &&
            a.collisionBoxX + a.collisionBoxWidth > b.collisionBoxX &&
            a.collisionBoxY < b.collisionBoxY + b.collisionBoxHeight &&
            a.collisionBoxY + a.collisionBoxHeight > b.collisionBoxY);
}


void jugar() {
    cout << "Iniciando juego..." << endl;

    ALLEGRO_DISPLAY* display = al_get_current_display();
    ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());

    al_clear_to_color(al_map_rgb(0, 0, 0)); // Limpia la pantalla

    Character player;
    player.x = 500;
    player.y = 750;
    player.speed = 12.5;
    player.width = 100;
    player.height = 100;
    player.collisionBoxX = player.x;
    player.collisionBoxY = player.y;
    player.collisionBoxWidth = player.width;
    player.collisionBoxHeight = player.height;
    int vidas = 3;
    Character enemy;
    enemy.x = player.x + 200;
    enemy.y = player.y;
    enemy.speed = 3;
    enemy.width = 100;
    enemy.height = 100;
    enemy.collisionBoxX = enemy.x;
    enemy.collisionBoxY = enemy.y;
    enemy.collisionBoxWidth = enemy.width;
    enemy.collisionBoxHeight = enemy.height;


    // Carga la imagen desde un archivo en el directorio actual
    player.image = al_load_bitmap("personaje1.png");
    if (!player.image) {
        fprintf(stderr, "Error: No se pudo cargar la imagen.\n");
        return;
    }
    enemy.image = al_load_bitmap("enemigo.png");
    ALLEGRO_BITMAP* background = al_load_bitmap("nivel2.jpg");
    if (!background) {
        fprintf(stderr, "Error: No se pudo cargar la imagen de fondo.\n");
        return;
    }

    al_set_target_backbuffer(display);

    al_flip_display();

    bool running = true;
    while (running) {
        drawBackground(background);
        drawCharacter(player);
        drawCharacter(enemy);
        moveTowards(enemy, player);
        if (checkCollision(player, enemy)) {
            // Aquí puedes manejar la lógica de la colisión, como disminuir la salud del jugador, etc.
            // Por ejemplo, imprimir un mensaje y salir del bucle para finalizar el juego.
            cout << "¡Colisión con el enemigo!" << endl;
            vidas = vidas - 1;
            cout << endl << "vidas: " << vidas << endl;
            player.x = 100;
            player.collisionBoxX = player.x;
            //player.y = 900;
            if (vidas == 0) {
                partidas++;
                cout << partidas;
                ofstream archivoSalida("contador.txt");
                if (archivoSalida.is_open()) {
                    // Escribe la variable en el archivo
                    archivoSalida << partidas << endl;

                    // Cierra el archivo
                    archivoSalida.close();
                    cout << "Variable guardada exitosamente en el archivo." << endl;
                }
                else {
                    cerr << "No se pudo abrir el archivo para escritura." << endl;
                }
                running = false;

            }
            
        }
        //drawMenu(nullptr, -1);


        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_A:
                player.x -= player.speed;
                player.collisionBoxX = player.x;
                player.collisionBoxY = player.y;
                break;
            case ALLEGRO_KEY_D:
                player.x += player.speed;
                player.collisionBoxX = player.x;
                player.collisionBoxY = player.y;
                break;
            }
        }

        al_flip_display();
    }

    al_destroy_bitmap(player.image);
    al_destroy_event_queue(eventQueue);
}
void contador(int contador) {
    // Limpia la pantalla
    al_clear_to_color(al_map_rgb(0, 0, 0));

    ALLEGRO_BITMAP* pantalla_contador = al_load_bitmap("pantalla_contadores.jpg");

    // Dibuja el texto de las indicaciones
    al_draw_bitmap(pantalla_contador, 0, 0, 0);
    // Crea un objeto de fuente
    ALLEGRO_FONT* font = al_create_builtin_font();

    // Dibuja el contador en la pantalla
    al_draw_textf(font, al_map_rgb(0, 0, 0), SCREEN_WIDTH / 2, 300, ALLEGRO_ALIGN_CENTER, "Partidas Jugadas: %d", contador);

    // Actualiza la pantalla
    al_flip_display();

    // Espera durante un tiempo antes de continuar (puedes ajustar el tiempo según tus necesidades)
    al_rest(5.0);

}
int salir() {
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_BITMAP* pantalla_salir = al_load_bitmap("pantalla_salir.jpg");

    ALLEGRO_FONT* font = al_create_builtin_font();
    true;
    while (true) {
        //ALLEGRO_EVENT event;
        //al_wait_for_event(queue, &event);
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(pantalla_salir, 0, 0, 0);
        al_flip_display();
        //cout << "jugar";
    }
    return 1;

}
void personaje() {
    // Limpia la pantalla
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // Crea un objeto de fuente
    ALLEGRO_BITMAP* pantalla_personaje = al_load_bitmap("pantalla_personaje.jpg");

    // Dibuja el texto de las indicaciones
    al_draw_bitmap(pantalla_personaje, 0, 0, 0);
    // Actualiza la pantalla
    al_flip_display();

    // Espera durante un tiempo antes de continuar
    al_rest(3.0);


}
void indicaciones() {
    // Limpia la pantalla
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // Crea un objeto de fuente
    ALLEGRO_BITMAP* pantalla_indicaciones = al_load_bitmap("pantalla_indicaciones.jpg");

    // Dibuja el texto de las indicaciones
    al_draw_bitmap(pantalla_indicaciones, 0, 0, 0);
    // Actualiza la pantalla
    al_flip_display();

    // Espera durante un tiempo antes de continuar
    al_rest(5.0);

}

int main()
{
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    ALLEGRO_FONT* font = al_create_builtin_font();
    must_init(font, "font");
    must_init(al_init_primitives_addon(), "primitives");

    if (!al_init())
    {
        printf("couldn't initialize allegro\n");
        return 1;
    }

    if (!al_install_keyboard())
    {
        printf("couldn't initialize keyboard\n");
        return 1;
    }

    ALLEGRO_DISPLAY* disp = al_create_display(887, 885);
    if (!disp)
    {
        printf("couldn't initialize display\n");
        return 1;
    }




    if (!al_init_image_addon()) {
        printf("couldn't initialize addon\n");
        return 1;
    }
    al_install_mouse();
    must_init(al_install_mouse(), "mouse");

    ALLEGRO_BITMAP* menu = al_load_bitmap("menu_null.jpg");
    ALLEGRO_BITMAP* menu_jugar = al_load_bitmap("menu_jugar.jpg");
    ALLEGRO_BITMAP* menu_salir = al_load_bitmap("menu_salir.jpg");
    ALLEGRO_BITMAP* menu_contador = al_load_bitmap("menu_contador.jpg");
    ALLEGRO_BITMAP* menu_personaje = al_load_bitmap("menu_personaje.jpg");
    ALLEGRO_BITMAP* menu_indicaciones = al_load_bitmap("menu_indicaciones.jpg");
    ALLEGRO_BITMAP* nivel1 = al_load_bitmap("nivel1.jpg");
    ALLEGRO_BITMAP* nivel2 = al_load_bitmap("nivel2.jpg");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());

    al_hide_mouse_cursor(disp);

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;
    float x, y;
    x = 100;
    y = 100;

    al_start_timer(timer);
    while (1)
    {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:

            redraw = true;
            break;

        case ALLEGRO_EVENT_MOUSE_AXES:
            x = event.mouse.x;
            y = event.mouse.y;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:

            break;

        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            if (x > 285 && x < 597 && y < 189 && y > 125) {
                al_draw_bitmap(menu_jugar, 0, 0, 0);
                if (event.mouse.button & 1) {
                    ifstream archivoEntrada("contador.txt");


                    if (archivoEntrada.is_open()) {
                        archivoEntrada >> partidas;
                        archivoEntrada.close();
                        cout << "Archivo abierto correctamente." << endl;
                    }
                    else {
                        cout << "No se pudo abrir el archivo para lectura." << endl;
                        return 1;
                    }
                    jugar();
                }
            }
            else if (x > 285 && x < 597 && y < 296 && y > 230) {
                al_draw_bitmap(menu_salir, 0, 0, 0);
                if (event.mouse.button & 1) {
                    al_destroy_display(disp);
                    return 0;
                }
            }
            else if (x > 285 && x < 597 && y < 410 && y > 344) {
                al_draw_bitmap(menu_contador, 0, 0, 0);
                if (event.mouse.button & 1) {
                    ifstream archivoEntrada("contador.txt");
                    

                    if (archivoEntrada.is_open()) {
                        archivoEntrada >> partidas;
                        archivoEntrada.close();
                    }
                    else {
                        cerr << "No se pudo abrir el archivo para lectura." << endl;
                        return 1;
                    }
                    contador(partidas);
                }
            }
            else if (x > 285 && x < 597 && y < 522 && y > 458) {
                al_draw_bitmap(menu_indicaciones, 0, 0, 0);
                if (event.mouse.button & 1) {
                    indicaciones();
                }
            }
            else if (x > 285 && x < 597 && y < 635 && y > 569) {
                al_draw_bitmap(menu_personaje, 0, 0, 0);
                if (event.mouse.button & 1) {
                    personaje();
                }
            }
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        }

        if (done)
            break;

        if (redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(menu, 0, 0, 0);

            
            if (x > 285 && x < 597 && y < 189 && y > 125) {
                al_draw_bitmap(menu_jugar, 0, 0, 0);
                if (event.mouse.button & 1) {
                    jugar();
                }
            }
            else if (x > 285 && x < 597 && y < 296 && y > 230) {
                al_draw_bitmap(menu_salir, 0, 0, 0);
                if (event.mouse.button & 1) {
                    salir();
                }
            }
            else if (x > 285 && x < 597 && y < 410 && y > 344) {
                al_draw_bitmap(menu_contador, 0, 0, 0);
                if (event.mouse.button & 1) {
                    //contador();
                }
            }
            else if (x > 285 && x < 597 && y < 522 && y > 458) {
                al_draw_bitmap(menu_indicaciones, 0, 0, 0);
                if (event.mouse.button & 1) {
                    //contador();
                }
            }
            else if (x > 285 && x < 597 && y < 635 && y > 569) {
                al_draw_bitmap(menu_personaje, 0, 0, 0);
                if (event.mouse.button & 1) {
                    //contador();
                }
            }
           

            //al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "X: %.1f Y: %.1f", x, y);
            al_draw_filled_rectangle(x, y, x + 10, y + 10, al_map_rgb(255, 0, 0));
            al_flip_display();



            redraw = false;
        }
    }
    al_destroy_bitmap(menu);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}

