package afester.logging;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class ColorizedLog {
	
    protected static final Logger log = LogManager.getLogger();

	public static void main(String[] args) {
		log.trace("Trace Message");
		log.debug("Debug Message");
		log.info("Info Message");
		log.warn("Warning Message");
		log.error("Error Message");
		log.fatal("Fatal error message");
	}
}
