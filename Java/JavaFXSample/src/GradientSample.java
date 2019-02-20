import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.image.PixelReader;
import javafx.scene.image.WritableImage;
import javafx.scene.paint.Color;
import javafx.scene.paint.CycleMethod;
import javafx.scene.paint.LinearGradient;
import javafx.scene.paint.Stop;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Font;
import javafx.scene.text.Text;
import javafx.stage.Stage;

public class GradientSample extends Application {
    public static void main(String[] args) { launch(args); }

    @Override
    public void start(Stage primaryStage) {

        Group mainGroup = new Group();
        Rectangle r = new Rectangle(0, 0, 200, 200);

        Stop[] stops = new Stop[] { new Stop(0, new Color(0.0, 0.0, 1.0, 1.0)), 
                                    new Stop(1, new Color(1.0, 1.0, 1.0, 1.0)) };
        LinearGradient lg = new LinearGradient(0, 0, 200, 200, false, 
                                               CycleMethod.NO_CYCLE,
                                               stops);
        r.setFill(lg);

        mainGroup.getChildren().addAll(r);
        Scene scene = new Scene(mainGroup, 200, 200);
        primaryStage.setScene(scene);
        primaryStage.show();
    }
/*    
        Text t = new Text(20, 40, "TESTTEXT");
        Font font = Font.font(t.getFont().getFamily(), 24);
        t.setFont(font);

        WritableImage image = scene.snapshot(null);
        PixelReader pixelReader = image.getPixelReader();
        System.out.println("Color: " + pixelReader.getColor((int)image.getWidth()/2, (int)image.getHeight()/2));
    @Override
    public void start(Stage primaryStage) {
        
        System.err.println("prism.order:" + System.getProperty("prism.order"));

        Group mainGroup = new Group();
        Rectangle r = new Rectangle(0, 0, 200, 200);

        Stop[] stops = new Stop[] { new Stop(0, new Color(0.0, 0.0, 1.0, 1.0)), 
                                    new Stop(1, new Color(1.0, 1.0, 1.0, 0.0)) };
        LinearGradient lg = new LinearGradient(0, 0, 1, 1, true, CycleMethod.NO_CYCLE, stops);
        r.setFill(lg);

        mainGroup.getChildren().add(r);
        Scene scene = new Scene(mainGroup, 200, 200);
        scene.setFill(Color.TRANSPARENT);
        primaryStage.setScene(scene);
        primaryStage.show();

        WritableImage image = scene.snapshot(null);
        PixelReader pixelReader = image.getPixelReader();

        System.out.println("Color: " + pixelReader.getColor((int)image.getWidth()/2, (int)image.getHeight()/2));
    }*/
}