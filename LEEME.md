twitter-cli
===========

Este pequeño programa es un cliente de Twitter en modo texto que utiliza el
API de Twitter para enviar tweets y ver el timeline del usuario.

El API de Twitter es un API REST bastante genérica, que utiliza sobre todo
peticiones GET y POST para comunicarse con ella.

Twitter, al igual que otros muchos servicios, utiliza como tecnologías
subyacentes para comunicarse con el API tanto OAuth para autorización como
JSON para representar los datos devueltos.


JSON
----

JSON viene de *JavaScript Object Notation*, y es una notación para escribir
estructuras de datos que es muy parecida a la utilizada para realizar esto
mismo en JavaScript/ECMAScript.

JSON es un formato que tiene dos tipos de datos compuestos y unos cuantos
simples. A continuación se detallan estos tipos de datos:

  - Objetos: `{}` , `{ "clave" : "valor" }
  - Arrays: `[]`, `[ "valor1", 2, 3, 4.0 ]`
  - Strings: `"hola mundo"`
  - Números: `4`, `5.2`, `-5.2e8`
  - Booleanos: `false`, `true`
  - El valor nulo: `null`

Para trabajar con esta estructura hemos utilizado un analizador sintáctico
generado con Flex y Bison que realiza la labor de identificar cada valor de
JSON y almacenarlo en una estructura propia que utilizamos para gestionar
tanto la información del API de Twitter como nuestro propio fichero de
configuración.


OAuth
-----

OAuth es un protocolo de autenticación que se utiliza bastante en APIs, debido
a la naturaleza *stateless* (sin estado) del protocolo HTTP.

Básicamente, a partir de dos pares de claves se firma un mensaje. La firma ha
de validarla Twitter para que la petición se considere válida. Esto permite
evitar ataques de replay o de spoofing cambiando parámetros al azar en el
mensaje, siempre que el atacante no conozca la clave privada con la que se
firma.

Una de estas claves la proporciona Twitter y es invariante para la aplicación,
y la otra se obtiene autenticando al usuario contra Twitter en el proceso
de autorización.

Para implementar OAuth hemos delegado el cálculo de las sumas de comprobación
y de la generación de parámetros a libOAuth, una biblioteca en C creada para
esto.

Como los ficheros de cabeceras de liboauth no se encuentran en los ordenadores
de los laboratorios, hemos incluido una versión de liboauth en el paquete de
este software para no tener esta dependencia externa.


libcurl
-------

Ya que la aplicación ha de comunicarse con Twitter en tiempo real, hemos de
realizar estas peticiones en el programa. Para ello hemos utilizado una
biblioteca C para realizar peticiones remotas: libcurl.

Debido a que los ficheros de cabeceras de libcurl tampoco están instalados en
los ordenadores de los laboratorios, también hemos incluido una versión de
libcurl para poder compilar nuestro programa sin esta dependencia.


Compilación
-----------

En vista a la posibilidad de uso en multiplataforma de este programa, hemos
decidido utilizar para la gestión de la compilación las **GNU Build Tools**,
también conocidas como las *autotools*.

Estas herramientas nos permiten generar unos ficheros autocontenidos para
compilar el programa en cualquier máquina (aunque no tengan las autotools)
de un modo bastante estándar, concretamente, los pasos a seguir son los
siguientes:

    cd path/to/twitter-cli
    ./configure [--enable-unicode] [--prefix=/usr/local]
    make
    # Opcionalmente: make install


Configuración
-------------

El software necesita un fichero de configuración para guardar el último
token de oauth válido. Al inicio se autocomprueba su validez mediante una
*API Call* a Twitter, y en caso de no ser válido se procede como si no hubiese
fichero de configuración: Se le solicita al usuario que entre en twitter
mediante la URL que se ofrece por la salida estándar.

Una vez en Twitter, y autorizada la aplicación, el servicio web proporcionará
un número PIN. Al introducirlo por la entrada estándar en la aplicación ya
podremos acceder al menú.


Conclusiones
------------

El haber realizado un programa como este nos ha permitido por una parte
valorar y conocer más a fondo todavía las herramientas flex y bison (por
ejemplo, cómo cambiar el buffer de entrada para analizar las respuestas del
API, o el fichero de configuración).

Por otra parte también hemos utilizado un servicio externo, con lo que es
posible que en un futuro sigamos desarrollando alguna funcionalidad más para
la aplicación.

En vistas a esto, hemos publicado el código fuente bajo una licencia libre.
