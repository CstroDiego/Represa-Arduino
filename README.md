# Represa-Arduino
Este proyecto consiste en un código de Arduino que se utiliza para enviar datos de un sensor a una aplicación Android. Los datos del sensor se envían a través de un módulo Bluetooth HC-05 y son graficados en tiempo real en la aplicación.

## Descripción del Código

El código de Arduino se encarga de las siguientes funcionalidades:

- Configuración de pines: Se definen los pines utilizados para los LEDs, el pushbutton y el sensor.
- Comunicación Bluetooth: Se establece la comunicación con el módulo Bluetooth HC-05 a través de la librería `SoftwareSerial`.
- Manipulación de datos: Se utiliza la librería `ArduinoJson` para crear y manipular un documento JSON que almacena los datos del sensor.
- Control de tiempo: Se utilizan variables y constantes para controlar el tiempo de lectura del sensor, envío del documento JSON y detección de inactividad.
- Modo de ahorro de energía: Se implementa un modo de ahorro de energía que entra en acción después de un período de inactividad y se puede salir presionando un botón.

El código se organiza en una función `setup()` que se ejecuta una vez al iniciar el Arduino y una función `loop()` que se ejecuta de forma repetida.

## Tecnologías Utilizadas

- Arduino: El código está escrito en lenguaje de programación de Arduino.
- Librería SoftwareSerial: Se utiliza para establecer la comunicación con el módulo Bluetooth HC-05 a través de pines digitales.
- Librería ArduinoJson: Se utiliza para la manipulación de datos en formato JSON.

## Configuración y Uso

1. Conecta el módulo Bluetooth HC-05 al Arduino según las especificaciones.
2. Carga el código en tu placa de Arduino.
3. Asegúrate de tener la aplicación Android adecuada para recibir y graficar los datos del sensor.

## Contribuciones y Mejoras

Este proyecto es de código abierto y se anima a la comunidad a contribuir con ideas, mejoras y correcciones. Si tienes alguna sugerencia para mejorar la funcionalidad, el rendimiento o cualquier otro aspecto del proyecto, no dudes en enviar una solicitud de extracción. Tu participación es fundamental para hacer de esta aplicación una herramienta más versátil y eficiente.

## Licencia

Este proyecto se distribuye bajo la licencia MIT. Consulta el archivo [LICENSE](URL_DEL_ARCHIVO_LICENSE) para obtener más detalles.

