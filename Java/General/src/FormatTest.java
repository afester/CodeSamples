import java.math.BigDecimal;
import java.text.DecimalFormat;

public class FormatTest {

    public static void main(String[] args) {
        System.out.print(System.getProperty("java.runtime.version") + ": ");
        System.out.println(new DecimalFormat().format(new BigDecimal(5.1)));
    }
}
