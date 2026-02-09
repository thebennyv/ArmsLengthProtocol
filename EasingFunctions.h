#ifndef EASINGFUNCTIONS_H
#define EASINGFUNCTIONS_H

#include <cmath>

namespace Easing {

// In each easing function below, the input x takes a floating point value
// between 0.0 and 1.0, and likewise the return value.
// 
// easeInOutQuad
//    1.00 ┼                                          ╭─────── 
//    0.90 ┤                                    ╭─────╯        
//    0.80 ┤                                ╭───╯              
//    0.70 ┤                             ╭──╯                  
//    0.60 ┤                          ╭──╯                     
//    0.50 ┤                       ╭──╯                        
//    0.40 ┤                    ╭──╯                           
//    0.30 ┤                 ╭──╯                              
//    0.20 ┤             ╭───╯                                 
//    0.10 ┤       ╭─────╯                                     
//    0.00 ┼───────╯                                           
// 
// easeInOutCubic
//    1.00 ┼                                      ╭─────────── 
//    0.90 ┤                                 ╭────╯            
//    0.80 ┤                              ╭──╯                 
//    0.70 ┤                           ╭──╯                    
//    0.60 ┤                         ╭─╯                       
//    0.50 ┤                        ╭╯                         
//    0.40 ┤                      ╭─╯                          
//    0.30 ┤                   ╭──╯                            
//    0.20 ┤                ╭──╯                               
//    0.10 ┤           ╭────╯                                  
//    0.00 ┼───────────╯                                       
// 
// easeInCubic
//    1.00 ┼                                                 ╭ 
//    0.90 ┤                                               ╭─╯ 
//    0.80 ┤                                             ╭─╯   
//    0.70 ┤                                           ╭─╯     
//    0.60 ┤                                        ╭──╯       
//    0.50 ┤                                      ╭─╯          
//    0.40 ┤                                   ╭──╯            
//    0.30 ┤                               ╭───╯               
//    0.20 ┤                          ╭────╯                   
//    0.10 ┤                  ╭───────╯                        
//    0.00 ┼──────────────────╯                                
// 
// easeOutCubic
//    1.00 ┼                               ╭────────────────── 
//    0.90 ┤                       ╭───────╯                   
//    0.80 ┤                  ╭────╯                           
//    0.70 ┤              ╭───╯                                
//    0.60 ┤           ╭──╯                                    
//    0.50 ┤         ╭─╯                                       
//    0.40 ┤      ╭──╯                                         
//    0.30 ┤    ╭─╯                                            
//    0.20 ┤  ╭─╯                                              
//    0.10 ┤╭─╯                                                
//    0.00 ┼╯                                                  
//
//    <!DOCTYPE html>
//    <html>
//        <head>
//            <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
//            <meta charset="UTF-8">
//            <title>asciichart</title>
//            <script src="asciichart.js"> /* https://github.com/kroitor/asciichart */ </script>
//            <script type="text/javascript">
//                function easeInOutQuad(x) {
//                    return x < 0.5 ? 2 * x * x : 1 - Math.pow(-2 * x + 2, 2) / 2;
//                }
//                function easeInCubic(x) {
//                    return x * x * x;
//                }
//                function easeOutCubic(x) {
//                    return 1 - Math.pow(1 - x, 3);
//                }
//                function easeInOutCubic(x) {
//                    return x < 0.5 ? 4 * x * x * x : 1 - Math.pow(-2 * x + 2, 3) / 2;
//                }
//                var config = {
//                    height: 10,
//                }
//                var s0 = new Array (51)
//                for (var i = 0; i < s0.length; i++) s0[i] = easeInOutQuad(i / (s0.length-1))
//                console.log (asciichart.plot(s0, config))
//                
//                for (var i = 0; i < s0.length; i++) s0[i] = easeInOutCubic(i / (s0.length-1))
//                console.log (asciichart.plot(s0, config))
//                
//                for (var i = 0; i < s0.length; i++) s0[i] = easeInCubic(i / (s0.length-1))
//                console.log (asciichart.plot(s0, config))
//                
//                for (var i = 0; i < s0.length; i++) s0[i] = easeOutCubic(i / (s0.length-1))
//                console.log (asciichart.plot(s0, config))
//            </script>
//        </head>
//        <body>
//        </body>
//    </html>

inline double easeInOutQuad(double x)
{
    // https://easings.net/#easeInOutQuad
    return (x < 0.5) ? (2.0 * x * x)
                     : 1.0 - pow((-2.0 * x) + 2.0, 2.0) / 2.0;
}

inline double easeInOutCubic(double x)
{
    // https://easings.net/#easeInOutCubic
    return (x < 0.5) ? 4.0 * x * x * x
                     : 1.0 - pow(-2.0 * x + 2.0, 3.0) / 2.0;
}

inline double easeInCubic(double x)
{
    // https://easings.net/#easeInCubic
    return x * x * x;
}

inline double easeOutCubic(double x)
{
    // https://easings.net/#easeOutCubic
    return 1.0 - pow(1.0 - x, 3.0);
}


} // namespace Easing

#endif // EASINGFUNCTIONS_H
