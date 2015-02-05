package sample
 
import java.util.List
 
class Greeter {
  def greetThem(List<String> names) {
    for(name: names) {
      println(name.sayHello)
    }
  } 
 
  def sayHello(String name) {
    'Hello ' + name + '!'
  }
}