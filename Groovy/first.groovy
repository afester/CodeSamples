/**
 * A first, very simple example for the Groovy language
 */

rand = new Random(System.currentTimeMillis())

playing = true

while(playing) {
    println 'I have a number between 0 and 1000 - which one is it?'
    x = rand.nextInt(1001)
    
    guessing = true
    while(guessing) {
        guess = System.console().readLine 'Your guess? '
        guessedValue = Integer.parseInt(guess)
        if (guessedValue > x) {
           println 'too large'
        } else if (guessedValue < x) {
           println 'too small'
        } else {
           println 'you got it!'
           guessing = false
        }
    }
    
    answer = System.console().readLine 'Play again (y/n)?'
    if (!answer.equals('y')) {
        playing = false
    }
}
