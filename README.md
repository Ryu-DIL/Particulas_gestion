# Gestión de Partículas (Particulas_gestion)

Un sistema en C++ diseñado para la gestión, lectura y procesamiento de datos de partículas elementales y "partículas raras". Este proyecto facilita el análisis de datos físicos y la interacción a través de un menú configurado, siendo una herramienta excelente para aplicaciones en física y entornos de educación científica.

## 📁 Estructura del Repositorio

El proyecto se compone de los siguientes archivos:

- `Main.cpp`: Código fuente principal de la aplicación. Contiene la lógica del sistema y el flujo del programa.
- `particulas.txt`: Base de datos en texto plano de las partículas estándar.
- `particulasraras.txt`: Base de datos para el registro de partículas clasificadas como "raras".
- `menu.ini`: Archivo de configuración para inicializar los parámetros del menú de usuario.
- `particulasraras.ini`: Archivo de configuración específico para ajustar la lectura y manejo de las partículas raras.

## 🚀 Compilación y Ejecución

El código está estructurado para ejecutarse desde la terminal y es totalmente compatible con entornos Linux (como Kubuntu, Ubuntu, etc.) utilizando el compilador GNU C++.

### 1. Compilar el código
Abre tu terminal (Konsole o similar) en el directorio del repositorio y ejecuta:
g++ Main.cpp -o gestion_particulas
