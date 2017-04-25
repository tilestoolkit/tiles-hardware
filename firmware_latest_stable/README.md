# Tiles DEVICE

## Notes
- When programming a Square Prototype 2 Rev D (RED Square) the ON/OFF switch must be put on OFF.
- Set in the code the type of Square module you have. For RFDUINO-shield-based modules set the *IS_SHIELD* variable on *TRUE*. For Prot#2 RevD module set the *COMMON_ANODE* to *true*

## API for Tiles DEVICE
### Events
| ID | Name | Parameter 1 | Parameter 2 | Implemented | Comments |
|----|------|-------------|-------------|-------------|----------|
|Tile_XX | tap    | single, double |  | yes | |
|        | tilt   | | | yes| |
|        | shake  | | | yes| |
|        | rotate | clock, c_clock | | yes | clockwise and counter clockwise |
|        | drop | | | no | free fall event |
|        | lift | | | no |  |
|        | proximity | | | no | to be implemented with capacitive touch |

#### Example

Event generated when one Tiles is tapped twice
```javascript
"Tile_XX,tap,double"
```

### Commands
<table>
    <tr>
        <th>ID</th>
        <th>Name</th>
        <th>Parameter1</th>
        <th>Parameter2</th>
        <th>Implemented</th>
        <th>Comments</th>
    </tr><tr>
        <td>Tile_XX,</td>
        <td>led,</td>
        <td>off</td>
        <td>-</td>
        <td>yes</td>
        <td></td>
    </tr><tr>
        <td></td>
        <td></td>
        <td>on,</td>
        <td>red, green, blue, white</td>
        <td>yes</td>
        <td></td>
    </tr><tr>
        <td></td>
        <td></td>
        <td>on,</td>
        <td>{HEX}</td>
        <td>yes</td>
        <td>example: FF00FF=pink</td>
    </tr><tr>
        <td></td>
        <td></td>
        <td>blink,</td>
        <td>red, green, blue, white</td>
        <td>yes</td>
        <td></td>
    </tr><tr>
        <td></td>
        <td></td>
        <td>blink,</td>
        <td>{HEX}</td>
        <td>yes</td>
        <td></td>
    </tr><tr>
        <td></td>
        <td></td>
        <td>fade,</td>
        <td>red, green, blue, white</td>
        <td>only blue color implemented</td>
        <td></td>
    </tr><tr>
         <td></td>
         <td>haptic,</td>
         <td>long</td>
         <td>-</td>
         <td>yes</td>
         <td>long vibration</td>
     </tr><tr>
         <td></td>
         <td></td>
         <td>burst</td>
         <td>-</td>
         <td>yes</td>
         <td>short vibration</td>
     </tr>
     <tr></tr>
     <tr><td></td><td>sound</td></tr>
     
</table>

#### Example
Turn on LED with color RED on 'Tile_XX' send command:
```javascript
"Tile_XX,led,on,red"
```
or use HEX
```javascript
"Tile_XX,led,on,FF0000"
```
