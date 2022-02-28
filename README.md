Copyright © 2021-2022 LeftOvers Productions - Todos los derechos reservados :)

# _**Project Vs21**_

*Documento de diseño de videojuego*

### **Nombre de la empresa: _LeftOvers Productions_**

_Twitter_: @LeftOverProds

_Correo de contacto_ : leftover.prods@gmail.com

Versión 0.1 – 3 de febrero de 2022

<table>
  <tr>
    <td colspan = "2"> Resumen </td>
  </tr>
  <tr>
    <td> Generos: Beat 'em up </td>
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

Juego de lucha 2D 1vs1 con multijugador local en el que dos personajes se enfrentan en un escenario compuesto por varias plataformas. Cada personaje dispondrá de un ataque básico y tres ataques especiales únicos, además de la habilidad de saltar y bloquear ataques rivales. Los ataques desplazan al rival en mayor o menor medida. El objetivo principal es conseguir que el rival salga del escenario.

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


## <a name = "aspectos-generales">1. Aspectos generales</a>

Esperamos ofrecer al jugador la experiencia de ver a los personajes de Proyectos 1 (en caso de haberlos jugados, y si no tampoco importa) en un juego de lucha estilo Smash Bros con estética de pixelart. El jugador luchará con uno de los personajes contra otro jugador. El objetivo de los jugadores es intentar sacar al otro jugador fuera del mapa , utilizando los distintos ataques y habilidades que posee cada personaje , estos aumentan el porcentaje del jugador contrincante por lo que más fácil es sacarle de la pantalla. Mientras tanto, el otro jugador tiene que intentar permanecer en el mapa utilizando los saltos y las habilidades de recovery que ofrecen un desplazamiento similar al doble salto.

### <a name = "relato-breve"> 1.1 Relato breve y parcial de una partida típica</a>

Al ejecutar el juego, los jugadores verán la pantalla de menú principal en el que verán los botones de jugar, ajustes y salir al escritorio. En el menú de ajustes podrán modificar el tamaño de la pantalla, sonido, etc.; El de salir al escritorio cierra el juego.

Al pulsar el botón de jugar se abrirá el menú de selección de personajes, en el que se verán a todos los personajes implementados en el juego y un botón de seleccionar un personaje al azar. Cuando todos los jugadores hayan elegido y se haya pulsado el botón en pantalla de aceptar, empezará el combate.

Después de una breve cuenta atrás los jugadores serán capaces de moverse libremente por el escenario. Tendrán que atacarse el uno al otro para que el rival acumule daño y así sea más fácil sacarlo de la pantalla para derrotarlo.

Una vez haya un ganador se mostrará una pantalla de victoria mostrando al que haya ganado, y al pulsar cualquier botón se volverá a la selección de personaje.

## <a name = "Jugabilidad"> 2. Jugabilidad</a>

Los personajes se sitúan en un escenario 2D y tendrán que atacar al rival hasta conseguir expulsarlo de las plataformas que forman el escenario. Cuantos más ataques reciba un jugador, más vulnerable se vuelve y es más probable que acabe saliendo de la pantalla, por lo que el jugador tendrá que decidir cuándo atacar y cuándo esquivar para lograr ser el último en pie.

### <a name = "Mecánica"> 2.1 Mecánica</a>

####  <a name = "Mecánicas-personaje"> Mecánicas del personaje</a>

- Mecanicas generales:
  - Vida: cada jugador tiene X vidas, al caerse fuera del escenario pierde una vida y se renace en la parte superior de la escenario (si le quedan más vidas).
  - Porcentaje: Cada jugador tiene un porcentaje que comienza en 0% y sube con cada ataque recibido. A mayor porcentaje mayores las probabilidades de sacarlo del escenario.
  - Aire: Todos los movimientos son iguales y pueden realizarse tanto si el personaje está en el aire como si está en el suelo a excepción de los ataques hacia abajo.
  - Rebote contra el suelo: Si el personaje choca contra una superficie con la suficiente velocidad, este rebotará perdiendo un poco del impulso en el proceso.
  - HitLag: al producir una ataque fuerte se produce un tiempo determinado de paro de la escena para producir la sensación de un impacto fuerte.
  - Endlag/recuperación: El tiempo que tarda el jugador en poder volver a moverse y/o realizar una acción después de realizar un ataque.
  - Noqueado: Cuando recibes un golpe fuerte te empujará hacia atrás en parábola y te dejará en un estado de noqueado durante un tiempo. En este estado solo podrás moverte horizontalmente mientras caes. La distancia de la parábola y el tiempo noqueado vendrán dados por el Porcentaje del jugador y la fuerza del ataque que haya recibido.

- Movimientos generales: 
  - Ataque básico: todos los personajes tienen un ataque básico distinto.
    - Ataque neutral (A) en mando, (V, P) en teclado
    - Ataque lateral(←/→ + A) en mando, (A/D + V, ←/→ + P) en teclado
    - Ataque hacia abajo (↓ + A) en mando, (S+ V, ↓ + P) en teclado
    - Ataque hacia arriba (↑ + A) en mando, (W + V, ↑ + P) en teclado
    - Ataques aéreo: (en el aire + A) en mando, (en el aire + V, P) en teclado
  - Bloqueo: (↓) en mando, (S, ↓) en teclado
    - Los personajes pueden usar el movimiento de bloqueo para reducir el daño recibido y evitar el efecto de knockback y de lanzamiento de otro jugador. Para el equilibrio, el movimiento tiene un tiempo de cooldown. El personaje no podrá realizar otros ataques mientras.
  - Esquiva: (en el aire + ↓)
    - No recibe daño mientras está esquivando pero sólo es posible activar el movimiento de esquiva en el aire. Durante la esquiva, el personaje acelera hacia abajo y no podrá realizar otros ataques.
  - Saltos: (↑)
    - El jugador salta más o menos dependiendo del tiempo que se presione el botón. Hay múltiples saltos.
- Movimientos especiales:
  - Especial neutral (←/→ + B) en mando, (A/D + C, ←/→ + O) en teclado
  - Ataque lateral(←/→ + B) en mando, (A/D + C, ←/→ + O) en teclado
  - Ataque hacia abajo (↓ + B) en mando, (S+ C, ↓ + O) en teclado
  - Recovery: (↑ + B) en el mando, (W + C, ↑ + O) en teclado


####  <a name = "Mecánicas-escenario"> Mecánicas de escenario</a>

El escenario contará con dos tipos de suelo: plataformas y bases. La base será el suelo principal, más pegado a la parte de abajo del escenario. Las plataformas estarán flotando, y serán más pequeñas y finas. La principal diferencia consiste en que podrás bajar de las plataformas atravesándolas con el comando Abajo+Abajo y subir a ellas atravesándolas desde abajo, mientras que las bases no son atravesables.

####  <a name = "Controles"> Controles</a>

El menú inicial antes de una partida se controlará con los controles de dirección y la tecla Enter/A o Retroceso/B para entrar/salir de una opción como los ajustes o elegir un personaje.

En partida el jugador dispone de varios botones con los que atacar, defender y maniobrar por el escenario:

- Movimiento: Se realizará con los controles de dirección del control que esté usando el jugador (flechas/wasd en teclado, cruceta en mando).
  - Izquierda/derecha: Moverán al jugador en esa dirección.
  - Arriba: Hará que el jugador salte.
  - Abajo: El jugador bloqueará/esquivará. Pulsado dos veces rápidamente subido a una plataforma te bajará de ella.

- Ataques básicos: Los botones V/P en teclado y A en mando activarán los ataques básicos. Pueden combinarse con las teclas de movimiento.
- Ataques especiales:Los botones C/O en teclado y B en mando activarán los ataques básicos. Pueden combinarse con las teclas de movimiento.
- Menú de pausa: La tecla Retroceso en teclado y Start en mando activará la pestaña de pausa de partida.
- Taunt: La tecla T/K en teclado y Select en mando activará la animación de burla.

####  <a name = "Cámara"> Cámara</a>

Cámara estática con movimiento en el eje Y para mostrar a ambos personajes en caso de distanciamiento. La distancia intentará ser ni muy grande ni muy pequeña, a una distancia prudente en la que se puedan ver a ambos. Cuando los jugadores sean golpeados se añadirá una especie de temblor en la pantalla para dar feedback del golpe a los jugadores.

###  <a name = "Dinámica"> 2.2 Dinámica

Dos jugadores se enfrentan entre sí en un escenario, cada uno puede golpear y ser golpeado. Los dos jugadores empiezan con tres vidas. Si se recibe daño, la fuerza de los golpes de tu oponente hacia ti será mayor, hasta el punto en que uno de los dos caiga del escenario, y por tanto pierda una vida. Al final pierde quien se quede sin vidas.

 ###  <a name = "Estética"> 2.3 Estética</a>

Los personajes están extraídos de distintos juegos, por ello se han rediseñado para que tengan la misma estética: están hechos en pixel art y con proporciones estilo cartoon. Los colores son los originales de sus respectivos juegos, por lo que resultan coloridos en contraste con el fondo, que está menos saturado.

Durante la partida se pueden ver distintos efectos que hacen el juego más entretenido visualmente, como una cuenta atrás al inicio del combate, una cámara lenta al usar determinados ataques…

 ##  <a name = "Menús"> 3. Menús y modos de juego</a>

- Menú principal:
  - Jugar:
    - Selección de personaje
    - Selección de mapa
    - Empezar el combate
  - Configuración
  - Salir
- Menú de pausa
  - Reanudar
  - Configuración
  - Menú principal
- Menú de victoria
  - Marcadores
  - Rematch
  - Menú principal

###  <a name = "Configuración"> 3.1 Configuración</a>

- Sonido:
  - SFX
  - Música
- Pantalla
- Controles (Permite cambiar los controles, solo en el menú principal)

 ###  <a name = "Interfaz"> 3.2 Interfaz y control</a>

_Párrafos e ilustraciones donde se describe los controles que se pueden usar en el juego, los distintos menús &quot;in-game&quot; que encontramos, el head-up display (HUD) que pueda mostrarse durante la partida, etc._

 ##  <a name = "Contenido"> 4. Contenido</a>

El juego tendrá un fondo de escenario, música de batalla y de menú, sonidos de golpes dados y recibidos, así como de selección de botones, sprites y animaciones únicos de cada personaje y otras imágenes como fondo del menú y de la pestaña de ajustes.

###  <a name = "Historia"> 4.1 Historia</a>
  
_"6 junio de 2021, 14:00. Día tormentoso en San Francisco, California. Sobre la ciudad se cierne un gran nubarrón negro cual cuervo que extiende sus alas anunciando a los cuatro vientos un cambio, resonando con truenos que hacen temblar los cristales y brillando con rayos que parten árboles y funden tendidos eléctricos enteros. Uno de estos poderosos disparos de la naturaleza alcanza una sede de oficinas y servidores conocida por todos los alumnos de informática: la sede de Github Inc._

_6 de junio de 2021, 23:00. A una hora de la media noche en España. Nueve grupos de alumnos de Desarrollo de Videojuegos de la UCM se encuentran realizando los últimos commits de sus juegos de Proyectos 1. Por caprichos del destino todos pulsan el botón de Push a la vez, justo a la vez que el rayo impacta en los servidores de la compañía. Un resplandor blanco sale de las pantallas de los estudiantes, y cuando sus ojos se recuperan ante tal fogonazo de luz, a todos les sale el mismo error de Merge con 9000 cambios pero que está completamente en blanco. Cuando todos, asustados, se ponen a ejecutar sus juegos, se dan cuenta de que sus protagonistas no aparecen en pantalla…"_

El Ganso de la ComJamOn, la GameJam de la Universidad Complutense de Madrid, ha secuestrado a los personajes de los juegos de Proyectos 1 porque le pareció horrible que tan buenos juegos se desaprovecharan al no haber categoría a “Mejor juego del año de Proyectos 1”. Por ello, ha creado escenarios de combate en un juego aparte y ha traído a este a los personajes de los proyectos, para que peleen entre ellos y se decida por fin cuál es el mejor juego de Proyectos 1 de 2020/21.

###  <a name = "Personajes"> 4.2 Personajes</a>

Los personajes del juego son los protagonistas de los proyectos de la asignatura Proyectos 1 del curso 2020/2021.

#### -Makt Fange:

- Ataque básico:
  - Ataque neutral
  - Ataque lateral.
  - Ataque hacia abajo
  - Ataque hacia arriba
- Movimientos especiales:
  - Especial neutral: Ataque lento de daño alto con un área de impacto mayor a la de un ataque básico. Makt echa hacia atrás su brazo con la herropea, para a continuación arremeter contra el oponente con ella en la mano. Tiene recuperación media.
  - Especial lateral: Ataque cargado a distancia de daño medio-alto. Makt agarra la herropea por la cadena, haciendo la bola girar mientras el ataque se cargue. Cuando el botón de ataque sea soltado, Makt lanzará la bola en la dirección en la que ha cargado el ataque. Esta se detendrá al llegar al final de la distancia cargada por el lanzamiento o al chocar con un rival, haciéndole daño y empujándolo. La bola se quedará en el suelo hasta que Makt la recoja. Si esta cae fuera del mapa reaparecerá a los 3 segundos como si fuera un personaje. Este ataque tiene recuperación media.
  - Especial hacia abajo: Makt agarrará la herropea con ambas manos, pasándola por encima de su cabeza y bajándola con fuerza hacia el suelo, creando un área de daño medio en el suelo a su alrededor. Los oponentes afectados serán levantados en el aire durante una distancia corta determinada por el porcentaje de daño que tengan. Este ataque es lento y tiene recuperación media. Si Makt no tiene la herropea este comando la cogerá si Makt está lo suficientemente cerca.
  - Especial hacia arriba (Recovery): Lanzamiento hacia arriba. Makt gira la herropea con la cadena como si fuera el martillo de Thor y la lanza hacia arriba aún sujeta, impulsándose con ella por la inercia. Si la bola da a un rival no se detiene, pero este recibe daño igual.
  - Característica especial (Herropea): Makt es más pesado y lento cuando lleva la herropea en la mano, además de saltar un poco menos. El ataque Especial Lateral lanza la herropea, haciendo que esta se quede en el suelo. Mientras Makt no tenga la herropea será más ligero y podrá moverse más rápido y saltar más, pero no tendrá acceso a ataques especiales.

#### -Gato-Espía (Blink Master):

  - Ataque básico:
    - Ataque neutral
    - Ataque lateral.
    - Ataque hacia abajo
    - Ataque hacia arriba
  - Movimientos especiales:
    - Especial neutral: For his neutral special, he wields a G U N. Gato-Espía apuntará con su pistola, disparando una bala en la dirección en la que está mirando en ese momento. La bala viajará en una línea recta hasta que choque con un rival, haciendo daño bajo e interrumpiendo movimientos y ataques pero no knockeando, o alcance su distancia máxima. Si se deja pulsado, Gato-Espía dejará la pistola apuntando y la dirección de apuntado se podrá cambiar con las teclas de movimiento. Este ataque tiene rápida recuperación.
    - Especial lateral: Gato-Espía se teletransporta una corta distancia en la dirección pulsada.
    - Especial hacia abajo: Gato-Espía se teletransporta en el sitio en un pestañeo. Si recibiera algún ataque durante este teletransporte el daño es anulado, y si este ataque es cuerpo a cuerpo Gato-Espía aparecerá detrás del atacante, asestando un golpe de daño medio con su cuchillo.
    - Especial hacia arriba (Recovery): Gato-Espía se teletransporta una distancia corta hacia arriba.
    - Característica especial (Teletransporte): Gato-Espía tiene 3 cargas de teletransporte que puede usar para cualquiera de sus especiales de teletransporte. Estas cargas se regeneran con el tiempo. Si no tiene cargas, los especiales arriba, lateral y abajo no pueden hacerse. Los teletransportes laterales y hacia arriba pueden encadenarse, ya que al teletransportarte no hay endlag. Adicionalmente si mantienes pulsado el botón de Ataque Especial o lo pulsas al final del teletransporte, haces un poderoso ataque de cuchillo de recuperación alta y daño alto. Acertar el ataque te permite seguir teletransportándote, pero fallarlo te dejará en el estado de &quot;noqueado&quot; durante un tiempo determinado.

 ##  <a name = "Referencias"> 5. Referencias</a>

- [Super Smash Bros Ultimate](https://www.smashbros.com/en_GB/index.html) - 2019 (Nintendo)
- [Brawhalla](https://www.brawlhalla.com/)- 2014 (Blue Mammoth Games)
- [Rivals of Aether](https://rivalsofaether.com/) - 2017 (Dan Fornace)
- Datos completos del videojuego (u otra obra) tomado como referencia
- ...

 ##  <a name = "UML"> 6. UML</a>
 ![uml proyecto drawio](https://user-images.githubusercontent.com/63046549/154976798-93c78ab0-5171-43f2-ad1f-cfee1e613ae9.png)

Project VERSUS: 21 - Documento de diseño de videojuego / Biblia de producto Página 10 de 10
