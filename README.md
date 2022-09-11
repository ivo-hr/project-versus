Copyright © 2021-2022 LeftOvers Productions - Todos los derechos reservados :)

# _**Project Vs 21**_

*Documento de diseño de videojuego*

### **Nombre de la empresa: _LeftOvers Productions_**

_Página del juego_: https://ivo-hr.github.io/super-proj-melee/

_Twitter_: @LeftOverProds

_Correo de contacto_ : leftover.prods@gmail.com

Versión 0.1 – 3 de febrero de 2022

<table>
  <tr>
    <td colspan = "2"> Resumen </td>
  </tr>
  <tr>
    <td> Generos: Platform Fighter </td>
    <td> Modo: dos jugadores, 1v1</td>
  </tr>
  <tr>
    <td> Publico objetivo: <br>
         Edad: +13 <br>
         Sexo: Indiferente <br>
         Idioma: Español    
    </td>
    <td> Plataformas: Windows</td>
  </tr>
  <tr>
    <td> Cantidades: <br><br>
         Personajes: 2 <br>
   </td>
    <td> Hitos: <br>
           Fechas de Preproduccion: <br>
           Hito 0: 31/1 - 6/2 <br>
           Fechas de Producción: <br>
           Hito 1: 21/2 - 27/2 <br>
           Hito 2: 21/3 - 27/3 <br>
           Hito 3: 2/5 - 8/5 <br>
           Fecha de lanzamiento: mayo 2022 <br>
   </td>
  </tr>
</table>

## Descripción

Juego de lucha 2D 1vs1 con multijugador local en el que de dos a cuatro personajes se enfrentan en un escenario compuesto por varias plataformas. Cada personaje dispondrá de un ataque básico y tres ataques especiales únicos, además de la habilidad de saltar y bloquear ataques rivales. Los ataques desplazan al rival en mayor o menor medida. El objetivo principal es conseguir que el rival salga del escenario.

![captura](https://user-images.githubusercontent.com/82372508/167219888-166f5c6b-cab8-4dac-9fb1-773b0cf6a7a3.jpg)


## Versiones del documento

3 de febrero de 2022: Creación del documento

6 de febrero de 2022: Continuación del documento

21 de febrero de 2022: Implementación del documento en Readme.md

## Tabla de contenidos
1. [Aspectos Generales](#aspectos-generales)  
    1.1 [Relato breve y parcial de una partida típica](#relato-breve)  
  
2. [Jugabilidad](#Jugabilidad)  
    2.1 [Mecánica](#Mecánica)   
         - [Mecánicas del personaje](#Mecánicas-personaje)  
         - [Mecánicas del escenario](#Mecánicas-escenario)  
         - [Controles](#Controles)  
         - [Cámara](#Cámara)  
    2.2 [Dinámica](#Dinámica)  
    2.3 [Estética](#Estética)  

3. [Menús y modos de juegos](#Menús)  
    3.1 [Configuración](#Configuración)  
    3.2 [Interfaz y control](#Interfaz)  

4. [Contenido](#Contenido)  
    4.1 [Historia](#Historia)  
    4.2 [Personajes](#Personajes)  

5. [Referencias](#Referencias) 

6. [Diagrama UML](#UML)
7. [QA](#QA)


## <a name = "aspectos-generales">1. Aspectos generales</a>

Esperamos ofrecer al jugador la experiencia de ver a los personajes de Proyectos 1 (en caso de haberlos jugados, y si no tampoco importa) en un juego de lucha estilo Smash Bros con estética de pixelart. El jugador luchará con uno de los personajes contra otro jugador. El objetivo de los jugadores es intentar sacar al otro jugador fuera del mapa , utilizando los distintos ataques y habilidades que posee cada personaje , estos aumentan el porcentaje del jugador contrincante por lo que más fácil es sacarle de la pantalla. Mientras tanto, el otro jugador tiene que intentar permanecer en el mapa utilizando los saltos y las habilidades de recovery que ofrecen un desplazamiento similar al doble salto.

### <a name = "relato-breve"> 1.1 Relato breve y parcial de una partida típica</a>

Al ejecutar el juego, los jugadores verán la pantalla de menú principal en el que verán un mensaje que pide pulsar un determinado botón del mando/teclado que se va a utilizar, el botón de ajustes y el botón de salir al escritorio. En el menú de ajustes podrán modificar diferentes valores de sonido y verán una imagen con los diferentes controles disponibles (teclado, mando de NES y otro tipo de mando). El botón de salir al escritorio cierra el juego. 

Al pulsar el botón de jugar se abrirá el menú de selección de personajes, en el que se verán a todos los personajes implementados en el juego y un botón de seleccionar un personaje al azar. Cuando todos los jugadores hayan elegido y se haya pulsado el botón en pantalla de aceptar, empezará el combate.

Después de una breve cuenta atrás los jugadores serán capaces de moverse libremente por el escenario. Tendrán que atacarse el uno al otro para que el rival acumule daño y así sea más fácil sacarlo de la pantalla para derrotarlo.

Una vez haya un ganador se mostrará una pantalla de victoria mostrando al que haya ganado, y al pulsar el botón de volver a jugar se volverá al menú principal.

## <a name = "Jugabilidad"> 2. Jugabilidad</a>

Los personajes se sitúan en un escenario 2D y tendrán que atacar al rival hasta conseguir expulsarlo de las plataformas que forman el escenario. Cuantos más ataques reciba un jugador, más vulnerable se vuelve y es más probable que acabe saliendo de la pantalla, por lo que el jugador tendrá que decidir cuándo atacar y cuándo esquivar para lograr ser el último en pie.

### <a name = "Mecánica"> 2.1 Mecánica</a>

####  <a name = "Mecánicas-personaje"> Mecánicas del personaje</a>

- Mecanicas generales:
  - Vida: cada jugador tiene 3 vidas, al caerse fuera del escenario pierde una vida y se renace en la parte superior de la escenario (si le quedan más vidas).
  - Porcentaje: Cada jugador tiene un porcentaje que comienza en 0% y sube con cada ataque recibido. A mayor porcentaje mayores las probabilidades de sacarlo del escenario.
  - Aire: Todos los movimientos son iguales y pueden realizarse tanto si el personaje está en el aire como si está en el suelo a excepción de los ataques hacia abajo.
  - Rebote contra el suelo: Si el personaje choca contra una superficie con la suficiente velocidad, este rebotará perdiendo un poco del impulso en el proceso.
  - HitLag: al producir una ataque fuerte se produce un tiempo determinado de paro de la escena para producir la sensación de un impacto fuerte.
  - Endlag/recuperación: El tiempo que tarda el jugador en poder volver a moverse y/o realizar una acción después de realizar un ataque.
  - Noqueado: Cuando recibes un golpe fuerte te empujará hacia atrás en parábola y te dejará en un estado de noqueado durante un tiempo. En este estado solo podrás moverte horizontalmente mientras caes. La distancia de la parábola y el tiempo noqueado vendrán dados por el Porcentaje del jugador y la fuerza del ataque que haya recibido.

- Movimientos generales: 
  - Ataque básico: todos los personajes tienen un ataque básico distinto.
    - Ataque neutral (A) en mando, (LCTRL, RCTRL) en teclado
    - Ataque lateral(←/→ + A) en mando, (A/D + LCTRL, ←/→ + RCTRL) en teclado
    - Ataque hacia abajo (↓ + A) en mando, (S+ LCTRL, ↓ + RCTRL) en teclado
    - Ataque hacia arriba (↑ + A) en mando, (W + LCTRL, ↑ + RCTRL) en teclado
    - Ataques aéreo: (en el aire + A) en mando, (en el aire + LCTRL, en el aire + RCTRL) en teclado
  - Bloqueo: (↓) en mando, (S, ↓) en teclado
    - Los personajes pueden usar el movimiento de bloqueo para reducir el daño recibido y evitar el efecto de knockback y de lanzamiento de otro jugador. Para el equilibrio, el movimiento tiene un tiempo de cooldown. El personaje no podrá realizar otros ataques mientras.
  - Esquiva: (en el aire + ↓)
    - No recibe daño mientras está esquivando pero sólo es posible activar el movimiento de esquiva en el aire. Durante la esquiva, el personaje acelera hacia abajo y no podrá realizar otros ataques.
  - Saltos: (↑)
    - El jugador salta más o menos dependiendo del tiempo que se presione el botón. Hay múltiples saltos.
- Movimientos especiales:
  - Especial neutral (←/→ + B) en mando, (A/D + LSHIFT, ←/→ + RSHIFT) en teclado
  - Ataque lateral(←/→ + B) en mando, (A/D + LSHIFT, ←/→ + RSHIFT) en teclado
  - Ataque hacia abajo (↓ + B) en mando, (S+ LSHIFT, ↓ + RSHIFT) en teclado
  - Recovery: (↑ + B) en el mando, (W + LSHIFT, ↑ + RSHIFT) en teclado


####  <a name = "Mecánicas-escenario"> Mecánicas de escenario</a>

El escenario contará con dos tipos de suelo: plataformas y bases. La base será el suelo principal, más pegado a la parte de abajo del escenario. Las plataformas estarán flotando, y serán más pequeñas y finas. La principal diferencia consiste en que podrás bajar de las plataformas atravesándolas con el comando Abajo+Abajo y subir a ellas atravesándolas desde abajo, mientras que las bases no son atravesables.

####  <a name = "Controles"> Controles</a>

El juego está concebido para poder ser jugado con un mando de NES. Sin embargo, también daremos soporte a mandos actuales (como Xbox o PS4) y teclado.

El menú inicial antes de una partida se controlará con los controles de dirección y la tecla E/L/botón A o clicando sobre el botón de salir para entrar/salir de una opción como los ajustes o elegir un personaje.

En partida el jugador dispone de varios botones con los que atacar, defender y maniobrar por el escenario:

- Movimiento: Se realizará con los controles de dirección del control que esté usando 	el jugador (flechas/wasd en teclado, cruceta o joystick en mando).
  - Izquierda/derecha: Moverán al jugador en esa dirección.
  - Arriba: Hará que el jugador salte.
  - Abajo: El jugador bloqueará/esquivará. Pulsado dos veces rápidamente subido a una plataforma te bajará de ella.

- Ataques básicos: Los botones RCTRL/LCTRL en teclado y A en mando activarán los ataques básicos. Pueden combinarse con las teclas de movimiento.
- Ataques especiales:Los botones LSHIFT/RSHIFT en teclado y B en mando activarán los ataques básicos. Pueden combinarse con las teclas de movimiento.
- Menú de pausa: La tecla Escape en teclado activará la pestaña de pausa de partida.
- Taunt: La tecla T/J en teclado y Start en mando activará la animación de burla.

####  <a name = "Cámara"> Cámara</a>

Cámara dinámica que se mueve, acerca y aleja automáticamente para mantener en todo momento a todos los personajes en pantalla, en caso de distanciamiento. La distancia intentará ser ni muy grande ni muy pequeña, a una distancia prudente en la que se puedan ver a ambos. Cuando los jugadores sean golpeados se añadirá una especie de temblor en la pantalla para dar feedback del golpe a los jugadores.

###  <a name = "Dinámica"> 2.2 Dinámica

De dos a cuatro jugadores se enfrentan entre sí en un escenario, cada uno puede golpear y ser golpeado. Los jugadores empiezan con tres vidas. Si se recibe daño, la fuerza de los golpes de tu oponente hacia ti será mayor, hasta el punto en que uno de los dos caiga del escenario, y por tanto pierda una vida. Al final pierde quien se quede sin vidas.

 ###  <a name = "Estética"> 2.3 Estética</a>

Los personajes están extraídos de distintos juegos, por ello se han rediseñado para que tengan la misma estética: están hechos en pixel art y con proporciones estilo cartoon. Los colores son los originales de sus respectivos juegos, por lo que resultan coloridos en contraste con el fondo, que está menos saturado.

Durante la partida se pueden ver distintos efectos que hacen el juego más entretenido visualmente, como una cuenta atrás al inicio del combate, una cámara lenta al usar determinados ataques…

 ##  <a name = "Menús"> 3. Menús y modos de juego</a>

- Menú principal:
  - Jugar:
    - Selección de mapa
    - Selección de personaje
    - Empezar el combate
  - Configuración
  - Salir
- Menú de pausa
  - Reanudar
  - Configuración
  - Menú principal
- Menú de victoria
  - Marcadores
  - Menú principal
  
  Existen dos modos de juegos:
- Modo normal: se enfrentan de 2 a 4 jugadores en una batalla todos contra todos. El orden en el que vayan muriendo determinará su posición en el ranking final.
- Modo por equipos: dos equipos se enfrentan entre sí. Cada equipo puede estar formado por uno o dos jugadores. Los miembros del mismo equipo no pueden atacarse entre ellos. Cuando todos los miembros de un equipo hayan muerto, terminará la partida y el otro equipo habrá ganado.


###  <a name = "Configuración"> 3.1 Configuración</a>

- Sonido:
  - SFX
  - Música
- Controles (Permite ver los controles disponibles)

 ###  <a name = "Interfaz"> 3.2 Interfaz y control</a>

_Párrafos e ilustraciones donde se describe los controles que se pueden usar en el juego, los distintos menús &quot;in-game&quot; que encontramos, el head-up display (HUD) que pueda mostrarse durante la partida, etc._

 ##  <a name = "Contenido"> 4. Contenido</a>

El juego tendrá cuatro escenarios, una canción para menús y una para la batalla, 4 menús: menú inicial, menú de selección de personajes, menú de pausa en partida y menú post-partida; 4 personajes, cada uno con sonidos de golpes dados y recibidos y animaciones de movimiento, ataques básicos y especiales, noqueado, bloqueando, esquiva y taunt; dos tipos de controles: teclado y mando, teclado con opción de dos personas con el mismo teclado; una imagen de fondo de menú principal e imagenes de fondo y perfiles en la selección de personajes, además de imagen de victoria de cada personaje en el menú post-partida.

###  <a name = "Historia"> 4.1 Historia</a>
  
_"6 junio de 2021, 14:00. Día tormentoso en San Francisco, California. Sobre la ciudad se cierne un gran nubarrón negro cual cuervo que extiende sus alas anunciando a los cuatro vientos un cambio, resonando con truenos que hacen temblar los cristales y brillando con rayos que parten árboles y funden tendidos eléctricos enteros. Uno de estos poderosos disparos de la naturaleza alcanza una sede de oficinas y servidores conocida por todos los alumnos de informática: la sede de Github Inc._

_6 de junio de 2021, 23:00. A una hora de la media noche en España. Nueve grupos de alumnos de Desarrollo de Videojuegos de la UCM se encuentran realizando los últimos commits de sus juegos de Proyectos 1. Por caprichos del destino todos pulsan el botón de Push a la vez, justo a la vez que el rayo impacta en los servidores de la compañía. Un resplandor blanco sale de las pantallas de los estudiantes, y cuando sus ojos se recuperan ante tal fogonazo de luz, a todos les sale el mismo error de Merge con 9000 cambios pero que está completamente en blanco. Cuando todos, asustados, se ponen a ejecutar sus juegos, se dan cuenta de que sus protagonistas no aparecen en pantalla…"_

El Ganso de la ComJamOn, la GameJam de la Universidad Complutense de Madrid, ha secuestrado a los personajes de los juegos de Proyectos 1 porque le pareció horrible que tan buenos juegos se desaprovecharan al no haber categoría a “Mejor juego del año de Proyectos 1”. Por ello, ha creado escenarios de combate en un juego aparte y ha traído a este a los personajes de los proyectos, para que peleen entre ellos y se decida por fin cuál es el mejor juego de Proyectos 1 de 2020/21.

###  <a name = "Personajes"> 4.2 Personajes</a>

Los personajes del juego son los protagonistas de los proyectos de la asignatura Proyectos 1 del curso 2020/2021.

#### -Togo (DinoSouls):
  - Ataque básico:
    - Ataque neutral: Estocada de largo alcance con la lanza
    - Ataque lateral: Togo lanza su lanza en la dirección pulsada, y esta viaja en línea recta hasta alcanzar su distancia máxima, atravesando enemigos. No puede volver a hacer este movimiento hasta que la lanza haya desaparecido.
    - Ataque hacia abajo: Giro sobre sí mismo, dando con la capa primero en un lado y luego en otro.
    - Ataque hacia arriba: Un giro de lanza hacia arriba estilo aspas de helicóptero.
  - Movimientos especiales:
    - Especial neutral: Togo invoca el alma del Triceratops, generando un escudo en cúpula que devuelve proyectiles durante un segundo. Tras usarlo tendrás que esperar 3 segundos para poder usarlo de nuevo.
    - Especial lateral: Togo invoca el alma del Tiranosaurio, avanzando una pequeña distancia y lanzando un bocado en la dirección pulsada, haciendo daño medio. Tiene una recuperación media.
    - Especial hacia abajo: Togo invoca el alma del Anquilosaurio, arremetiendo a su alrededor con una cola que hace daño medio y tiene un gran noqueo. Tiene una recuperación media. (Dos hitbox, una a un lado y otra al otro, no simultaneas)
    - Especial hacia arriba (Recovery): Togo realiza un dash de media distancia hacia arriba.
  - Característica especial (Lanza): Togo posee una lanza con la que realiza ataques básicos de mayor alcance.


#### -Gato-Espía (Blink Master):

  - Ataque básico:
    - Ataque neutral: cuchillazo rápido y de corto alcance
    - Ataque lateral: dash lateral y cuchillazo de amplio rango.
    - Ataque hacia abajo: espagat y cascanueces en ambas direcciones
    - Ataque hacia arriba: un cuchillazo de amplio rango hacia arriba.
  Movimientos especiales:
    - Especial neutral: For his neutral special, he wields a G U N. Gato-Espía apuntará con su pistola, disparando una bala en la dirección en la que está mirando en ese momento. La bala viajará en una línea recta hasta que choque con un rival, haciendo daño bajo e interrumpiendo movimientos y ataques pero no knockeando, o alcance su distancia máxima. Si se deja pulsado, Gato-Espía dejará la pistola apuntando y la dirección de apuntado se podrá cambiar con las teclas de movimiento. Este ataque tiene rápida recuperación.
    - Especial lateral: Gato-Espía se teletransporta una corta distancia en la dirección pulsada.
    - Especial hacia abajo: Gato-Espía se teletransporta en el sitio en un pestañeo. Si recibiera algún ataque durante este teletransporte el daño es anulado, y si este ataque es cuerpo a cuerpo Gato-Espía aparecerá detrás del atacante, asestando un golpe de daño medio con su cuchillo.
    - Especial hacia arriba (Recovery): Gato-Espía se teletransporta una distancia corta hacia arriba.
    - Característica especial (Teletransporte): Gato-Espía tiene 3 cargas de teletransporte que puede usar para cualquiera de sus especiales de teletransporte. Estas cargas se regeneran con el tiempo. Si no tiene cargas, los especiales arriba, lateral y abajo no pueden hacerse. Los teletransportes laterales y hacia arriba pueden encadenarse, ya que al teletransportarte no hay endlag. Adicionalmente si mantienes pulsado el botón de Ataque Especial o lo pulsas al final del teletransporte, haces un poderoso ataque de cuchillo de recuperación alta y daño alto. Acertar el ataque te permite seguir teletransportándote, pero fallarlo te dejará en el estado de “noqueado” durante un tiempo determinado.

#### -Makt Fange:
  
  - Ataque básico:
    - Ataque neutral: puñetazo
    - Ataque lateral: cabezazo
    - Ataque hacia abajo: grito y salen zarigüeyas
    - Ataque hacia arriba: gancho con el puño
  - Movimientos especiales:
    - Especial neutral: Ataque lento de daño alto con un área de impacto mayor a la de un ataque básico. Makt echa hacia atrás su brazo con la herropea, para a continuación arremeter contra el oponente con ella en la mano. Tiene recuperación lenta.
    - Especial lateral: Ataque cargado a distancia de daño medio-alto. Makt agarra la herropea por la cadena, haciendo la bola girar mientras el ataque se cargue. Cuando el botón de ataque sea soltado, Makt lanzará la bola en la dirección en la que ha cargado el ataque. Esta se detendrá al llegar al final de la distancia cargada por el lanzamiento o al chocar con un rival, haciéndole daño y empujándolo bastante. La bola se quedará en el suelo hasta que Makt la recoja intentando hacer un ataque especial encima de ella. Si esta cae fuera del mapa reaparecerá a los 3 segundos como si fuera un personaje. Este ataque tiene recuperación media.
    - Especial hacia abajo: Makt agarrará la herropea con una mano, pasándola por encima de su cabeza y bajándola con fuerza hacia el suelo, creando un área de daño medio en el suelo a su alrededor. Los oponentes afectados serán levantados en el aire durante una distancia corta determinada por el porcentaje de daño que tengan. Este ataque es lento y tiene recuperación lenta.
    - Especial hacia arriba (Recovery): Lanzamiento hacia arriba. Makt gira la herropea con la cadena como si fuera el martillo de Thor y la lanza hacia arriba aún sujeta, impulsándose con ella por la inercia. Si la bola da a un rival no se detiene, pero este recibe daño leve igual.
  - Característica especial (Herropea): Makt es más pesado y lento cuando lleva la herropea en la mano, además de saltar un poco menos. El ataque Especial Lateral lanza la herropea, haciendo que esta se quede en el suelo. Mientras Makt no tenga la herropea será más ligero y podrá moverse más rápido y saltar más, pero no tendrá acceso a ataques especiales.   A DISCUTIR (El lanzamiento de herropea se hará con un ángulo de unos 30 grados y caerá de forma física haciendo un arco hasta que entre en contacto con el suelo o se caiga por el vacío.).

  
#### -NasNas:

  - Ataque básico (todos con el bastón):
    - Ataque neutral: estocada hacia la dirección en la que está mirando
    - Ataque lateral: barrido hacia la dirección en la que está mirando
    - Ataque hacia abajo: mini explosion por los extremos del bastón
    - Ataque hacia arriba: barrido hacia arriba
  - Movimientos especiales:
    - Especial neutral (efectos elementales bajos): 
      - Fuego: Dispara una bola de fuego de daño medio
      - Agua: Dispara una bola de agua de daño medio
      - Rayo: Dispara un rayo de daño bajo
    - Especial lateral (efectos elementales medios): 
      - Fuego: Dispara una llamarada de corto alcance, alto daño y amplia hitbox.
      - Agua: Dispara un chorro de agua de medio alcance, daño medio y media hitbox
      - Rayo: Dispara un rayo de largo alcance, daño bajo y estrecha hitbox.
    - Especial hacia abajo: Cambia entre los tres elementos de NasNas en orden (fuego, rayo y agua)
    - Especial hacia arriba (efectos elementales altos): Realiza un salto vertical y cae con fuerza al suelo, realizando una explosión en área del elemento que tenga en ese momento.
  - Característica especial (Ser rana y cambio de estado): NasNas, al ser una rana, tiene piernas que le otorgan una gran movilidad. Es más ágil y a diferencia del resto de luchadores tiene dos saltos en el aire, haciendo un total de tres.   
Además con su bastón controla tres elementos mágicos que producen efectos en el oponente.
    - Fuego: Hace daño por tiempo.
    - Agua: Ralentiza durante un tiempo.
    - Rayo: Inmoviliza durante un tiempo.  
Realizar combinaciones de estos efectos produce efectos adicionales, lo que hace que los dos efectos puestos se limpien instantáneamente.  
    - Fuego+Agua: Evapora el agua, realizando mucho daño
    - Fuego+Rayo: Crea una explosión que empuja al rival
    - Agua+Rayo: Electrocuta varias veces al oponente.
Todos estos hechizos dependen de una barra de maná que NasNas regenera con el tiempo (a razón de 5 de maná por segundo). Si tenemos en cuenta que la barra al máximo son 100, el especial neutral gasta 15, el especial lateral 25 y el especial arriba 50. Además, cada especial realiza el efecto del elemento de forma distinta. El especial neutral tendrá efectos de elemento más débiles, el especial lateral más fuerte y el máximo será el especial hacia arriba. Por ejemplo, en el caso del rayo el especial neutral paraliza durante 0.2 segundos, el especial lateral durante 0.6 y el especial hacia arriba durante 1 segundo. En el caso de las combinaciones su potencia viene determinada por el ataque que aplica el segundo efecto. Si el rival estuviera afectado por fuego y le asestamos un especial neutral de rayo, será empujado con menos fuerza que si le asestamos un especial lateral, y este con menos fuerza que el especial hacia arriba.

  #### -Ciro:
  
Ciro es un personaje oculto el cual tiene una pequeña probabilidad de aparecer al seleccionar la opción de personaje aleatorio.
  
  - Ataque básico: golpe de corto alcance.
  - Ataque especial: versión potenciada y más lenta del ataque básico la cual desplaza al rival una mayor distancia acompañado de un inconfundible grito de guerra.


 ##  <a name = "Referencias"> 5. Referencias</a>

- [Super Smash Bros Ultimate](https://www.smashbros.com/en_GB/index.html) - 2019 (Nintendo)
- [Brawhalla](https://www.brawlhalla.com/)- 2014 (Blue Mammoth Games)
- [Rivals of Aether](https://rivalsofaether.com/) - 2017 (Dan Fornace)
- Datos completos del videojuego (u otra obra) tomado como referencia
- ...

 ##  <a name = "UML"> 6. UML</a>
 ![uml proyecto drawio](https://user-images.githubusercontent.com/63046549/154976798-93c78ab0-5171-43f2-ad1f-cfee1e613ae9.png)

##  <a name = "QA"> 7. QA</a>

  - Primer QA
  
  IVÁN

¿Te ha parecido muy rápido el movimiento?

"No, no me ha parecido muy rápido. [...] 

Del 1 al 5, evalúa la dificultad del juego

"¿Eh, dificultad? 1 o 2, literalmente es moverte y dos tipos de ataque, o sea, no hay muchos tipos de ataque. Con la práctica cogerías habilidad…"

¿Qué cambiarías?

"Empezando porque te puedas salir de la partida con un doble salto, proyectiles más visibles y que hagan algo. Apenas tienen daño 

¿Qué es lo que más te ha gustado?

"El teletransporte está guay, está bien también el ataque cuerpo a cuerpo. No es que sea bueno, pero me gustaría saber qué hacen porque sólo una vez envíe al compañero a Parla. "

¿Y las que menos?

"Supongo que lo que he dicho antes del doble salto y los proyectiles y el mapa, no sé por qué estaba la plataforma porque te ibas siempre fuera del mapa."

VICTOR

¿Te ha parecido muy rápido el movimiento?

“Eh me ha parecido un poco difícil de controlar, no tanto por la velocidad sino porque te deslizas. Hay una fricción muy baja”

Del 1 al 5, evalúa la dificultad del juego

“Pelear contra Iván ha sido muy fácil con el Gato, con el Cubo ha sido muy complicado.”

¿Qué cambiarías?

“Movimiento no se sentía bien, añadir más personas, los mapas y ya”

¿Qué es lo que más te ha gustado?

“El teletransporte, es una mecánica interesante que no había visto antes en un fighter”

¿Y las que menos?

“Que si tocas el techo mueres y el deslizamiento. Las balas son muy chiquititas”
RAQUEL

¿Te ha parecido muy rápido el movimiento?

“No sé, me ha parecido normal”

Del 1 al 5, evalúa la dificultad del juego

“1, es muy fácil”

¿Qué cambiarías?
“Por mi, nada”

¿Qué es lo que más te ha gustado?
“La piedra que canta”

¿Y las que menos?
“Me parece que todo está bien, las líneas rojas tienen que ser más visibles. Y que haya un poco más de terreno para practicar.”







¿Cuánto tiempo dura el bucle de partida?
Minuto y medio, dos minutos
¿Cuánto tardan en aprender?
30 segundos de partida Victor, partida y media Iván
¿Qué mecánicas usan más?
Teletransporte, apertura de piernas???, corto hacia abajo, cuerpo a cuerpo Iván
¿Y cuáles no tanto?
Corto hacia arriba, disparo



Documentación general ->
	¿cuándo se han hecho, con cuánta gente, qué se les ha preguntado y qué ha salido?
	conclusión general de las pruebas

¿Cómo se aplica la info de QA al proyecto?
¿Qué cambios se han hecho?
¿Qué problemas o limitaciones se han descubierto?

  - Segundo QA
  
  Estimación partida: 5min
Básicos por partida: básico/2seg
Ataques fuertes por partida: ataq/5seg
Salto por partida: 2salto/seg
(Tp blinkmaster): 2tp/seg



Sergio

¿Te ha parecido muy rápido el movimiento?

A ver teóricamente el movimiento está bastante bien [...] en el menú tarda mucho en llegar. Respecto a los ataques te quedas demasiado bloqueado

Del 1 al 5, evalúa la dificultad del juego

“Como depende de personaje que tengas y contra quién es complicado decirte… Pero a la hora de aprender controles un 1.

¿Qué cambiarías?

“El tiempo de espera tras los ataques fuertes y el recover para volver a la plataforma (movilidad aérea). 

¿Qué es lo que más te ha gustado?

“La variedad de ataques que hay”

¿Y las que menos?

“El tiempo que tardas en recuperarte de un ataque fuerte con el cubo; rango de habilidad del cubo. El control que tienes sobre el ataque fuerte de Dino a los lados”

Gorka

¿Te ha parecido muy rápido el movimiento?

“En los menús muy lento, vendría bien movimiento dinámico (cuánto más mueves el joystick más se mueve el cursor). En el juego está bien”

Del 1 al 5, evalúa la dificultad del juego

“En cuanto a los controles, en teclado duro porque las manos están pegadas, en mando necesito un botón de salto apartado. Resulta bastante difícil volver a la plataforma si te caes. No queda del todo claro cuántos saltos te quedan”

¿Qué cambiarías?

“La movilidad en el aire y la fluidez de los ataques”

¿Qué es lo que más te ha gustado?

“La cara del cubo. La diversidad de ataques.”

¿Y las que menos?

“Que no sepas cuántos saltos tienes, que te pegues a las paredes (enlazado con lo anterior).”



¿Cuánto tiempo dura el bucle de partida?
Minuto y medio, dos minutos
¿Cuánto tardan en aprender?
30 segundos de partida Victor, partida y media Iván
¿Qué mecánicas usan más?
Teletransporte, apertura de piernas???, corto hacia abajo, cuerpo a cuerpo Iván
¿Y cuáles no tanto?
Corto hacia arriba, disparo



Documentación general ->
	¿cuándo se han hecho, con cuánta gente, qué se les ha preguntado y qué ha salido?
	conclusión general de las pruebas

¿Cómo se aplica la info de QA al proyecto?
¿Qué cambios se han hecho?
¿Qué problemas o limitaciones se han descubierto?

  
  
