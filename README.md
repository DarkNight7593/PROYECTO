# Plataforma de Streaming - Documentación

## Descripción
Este proyecto en C++ implementa una plataforma de streaming que permite a los usuarios buscar, ver detalles y marcar películas para ver más tarde o como favoritas. Además, ofrece recomendaciones basadas en las preferencias del usuario.

## Características
- **Búsqueda de películas:** Permite buscar películas por título o por tags.
- **Marcar películas:** Los usuarios pueden marcar películas para ver más tarde o como favoritas.
- **Recomendaciones:** Genera recomendaciones basadas en las películas que el usuario ha marcado como favoritas.
- **Interfaz de usuario:** Incluye un sistema de menús interactivo para la navegación.

## Estructura del Proyecto
El proyecto se divide en varias clases y funciones, cada una con una responsabilidad específica:

### Clases Principales
- **Pelicula:** Representa una película con sus atributos principales.
- **Trie:** Implementa una estructura de árbol trie para la búsqueda eficiente de películas por título o tags.
- **DatabaseIterator:** Facilita la paginación de los resultados de búsqueda.
- **Sesion:** Gestiona la sesión del usuario, incluyendo las listas de películas marcadas.
- **Web:** Clase abstracta que define la interfaz de la plataforma web.
- **MainWeb:** Implementa la interfaz Web y maneja la carga de datos y autenticación de usuarios.
- **ProxyWeb:** Controla el acceso a MainWeb, verificando permisos y país del usuario.
- **WebDecorator:** Clase base para los decoradores que añaden funcionalidades a la plataforma web.
- **MenuDecorator, BusquedanDecorator, RecomendacionDecorator, MastardeDecorator, MegustaDecorator:** Decoradores que añaden funcionalidades específicas como menús, busqueda, recomendaciones y listas de películas para ver mastarde y con likes.

### Funciones Principales
- **leerCSV:** Lee los datos de las películas desde un archivo CSV.
- **splitCSVLine:** Divide una línea de CSV en sus componentes.
- **limpiar:** Limpia la consola después de un cierto periodo de tiempo.
- **funcion:** Gestiona el flujo principal del menú de la plataforma.
### Configuración adicional en entorno de ejecución
- Activa "Emulate terminal in the output console" para asegurar una correcta visualización y funcionamiento del programa.
### Uso
Al iniciar el programa, se te pedirá que ingreses tu país (Peru) y ID (12345678) para verificar los permisos. Luego, podrás iniciar sesión con el usuario user y contraseña 123. Una vez dentro, podrás navegar por el menú y realizar diversas acciones como buscar películas, ver recomendaciones y gestionar tus listas de películas favoritas o para ver más tarde.

### Fuentes Bibliográficas
- **Estructuras de Datos:** El uso de tries para la búsqueda eficiente se basa en los principios discutidos en Introduction to Algorithms por Cormen, Leiserson, Rivest y Stein.
- **Patrones de Diseño:** Los patrones de diseño utilizados, como el Proxy y Decorator, están inspirados en Design Patterns: Elements of Reusable Object-Oriented Software por Gamma, Helm, Johnson y Vlissides.
- **Manejo de CSV en C++:** Para la lectura de archivos CSV, se ha seguido el enfoque descrito en C++ Cookbook por D. Ryan Stephens y Christopher Diggins.

### Video
- https://drive.google.com/drive/folders/1CVV4LKhp4ym9K9bXU_9EkI8P7lkHA6ZC
