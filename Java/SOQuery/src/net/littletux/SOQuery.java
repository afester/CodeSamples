package net.littletux;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.lang.reflect.Type;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.StringTokenizer;
import java.util.Vector;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;

public class SOQuery {

	public SOQuery() {
		try {
			// Load additional system properties.
			System.getProperties().load(new FileInputStream("soquery.properties"));
		} catch (FileNotFoundException e) {
			System.err.println("No configuration file found, using defaults");
		} catch (IOException e) {
			e.printStackTrace();
		}
	}


	public URL createQuery(String method, Map<String,String> params) throws MalformedURLException {
		String baseurl = "http://api.stackexchange.com/2.1/" + method + "?site=stackoverflow";

		for (Map.Entry<String,String> param : params.entrySet()) {
			String part = param.getKey() + "=" + param.getValue();
			baseurl = baseurl + "&" + part;
		}

		URL url = new URL(baseurl);
		return url;
	}

	public URL createQuery(String method) throws MalformedURLException {
		return createQuery(method, new HashMap<String,String>());
	}

/*
	public Object executeQuery1(URL url, Type clz) throws IOException {

		// Execute the query
		HttpURLConnection conn = (HttpURLConnection) url.openConnection();
		conn.connect();
		System.out.println("RC:" + conn.getResponseCode()); // 200
		System.out.println("RM:" + conn.getResponseMessage()); // "OK"

		StringBuffer result = new StringBuffer();
		String line = "";
		BufferedReader br = new BufferedReader(new InputStreamReader(
				conn.getInputStream()));
		while ((line = br.readLine()) != null) {
			result.append(line + "\n");
		}
		br.close();

		// deserialize the JSON response
		Gson gson = new Gson();
		return gson.fromJson(result.toString(), clz);
	}
*/
	public <T> T executeQuery(URL url, Type clz) throws IOException {

		// Execute the query
		HttpURLConnection conn = (HttpURLConnection) url.openConnection();
		conn.connect();
		System.out.println("RC:" + conn.getResponseCode()); // 200
		System.out.println("RM:" + conn.getResponseMessage()); // "OK"

		StringBuffer result = new StringBuffer();
		String line = "";
		BufferedReader br = new BufferedReader(new InputStreamReader(
				conn.getInputStream()));
		while ((line = br.readLine()) != null) {
			result.append(line + "\n");
		}
		br.close();

		// deserialize the JSON response
		Gson gson = new Gson();
		return gson.fromJson(result.toString(), clz);
	}
/*
	public <T> T executeQuery2(URL url) throws IOException {
		// Execute the query
		HttpURLConnection conn = (HttpURLConnection) url.openConnection();
		conn.connect();
		System.out.println("RC:" + conn.getResponseCode()); // 200
		System.out.println("RM:" + conn.getResponseMessage()); // "OK"

		StringBuffer result = new StringBuffer();
		String line = "";
		BufferedReader br = new BufferedReader(new InputStreamReader(
				conn.getInputStream()));
		while ((line = br.readLine()) != null) {
			result.append(line + "\n");
		}
		br.close();

		// deserialize the JSON response
		Gson gson = new Gson();
		Type dataType = new TypeToken<T>(){}.getType();
		System.err.println(dataType);
		return gson.fromJson(result.toString(), dataType);
	}
*/
	public void dumpBadges() throws IOException {
		
		int page = 1;

		Map<String,String> params = new HashMap<>();
		params.put("pagesize",  "100");
		params.put("order", "desc");
		params.put("sort",  "rank");

		boolean hasMore = true;
		while(hasMore) {
			URL url = createQuery("badges", params);
			Type dataType = new TypeToken<CommonWrapper<Badge>>(){}.getType();
			CommonWrapper<Badge> b = executeQuery(url, dataType);

			System.err.println(b);
			System.err.println(b.getItems().size());

			hasMore = b.hasMore();
			page++;
			params.put("page",  "" + page);
		}
	}

	public void dumpUser() throws IOException {
		URL url = createQuery("users/1611055");
		Type dataType = new TypeToken<CommonWrapper<User>>(){}.getType();
		CommonWrapper<User> u = executeQuery(url, dataType);

		System.err.println(u);
		System.err.println(u.getItems().size());
	}

	public static void main(String[] args) throws IOException {
		List selectedRowKeys = null;
		if (selectedRowKeys != null || selectedRowKeys.size() > 0) {
			System.out.println("Hello");
		}
/*		String rank = null; // card.substring(0,1);
	    //String suit = card.substring(1);
	    String cards = "A23456789TJQKDHSCl";
	    String[] name = {"Ace","Two","Three","Four","Five","Six","Seven","Eight","Nine","Ten","Jack","Queen","King","Diamonds","Hearts","Spades","Clubs"};
	    String c ="";
	    for(int a = 0, b = 1; a<cards.length()-1; b=a+1, a++){
	        if(rank==cards.substring(a,b)){
	            c+=name[a];
	        }


	    }
	    system.out.println(c);
	    /*
		byte[] buffer = {1,2,3,4,5};
		InputStream is = new ByteArrayInputStream(buffer);

		byte[] chunk = new byte[2];
		while(is.available() > 0) {
			int count = is.read(chunk);
			if (count == chunk.length) {
				System.out.println(Arrays.toString(chunk));
			} else {
				byte[] rest = new byte[count];
				System.arraycopy(chunk, 0, rest, 0, count);
				System.out.println(Arrays.toString(rest));
			} 
		}
/*		
		SOQuery soq = new SOQuery();

		try {
			soq.dumpBadges();
			//soq.dumpUser();
		} catch (IOException e) {
			e.printStackTrace();
		}

		/*
		 * // deserializing a single item String sample =
		 * "{\"badge_id\":1,\"rank\":\"bronze\",\"name\":\"Teacher\",\"award_count\":407685,\"badge_type\":\"named\",\"link\":\"http://stackoverflow.com/badges/1/teacher\"}"
		 * ; Item item = gson.fromJson(sample, Item.class);
		 * System.out.println(item.toString());
		 * 
		 * // deserialize an array of items String sample2 =
		 * "[{\"badge_id\":1,\"rank\":\"bronze\",\"name\":\"Teacher\",\"award_count\":407685,\"badge_type\":\"named\",\"link\":\"http://stackoverflow.com/badges/1/teacher\"},{\"badge_id\":2,\"rank\":\"bronze\",\"name\":\"Student\",\"award_count\":541987,\"badge_type\":\"named\",\"link\":\"http://stackoverflow.com/badges/2/student\"},{\"badge_id\":3,\"rank\":\"bronze\",\"name\":\"Editor\",\"award_count\":413414,\"badge_type\":\"named\",\"link\":\"http://stackoverflow.com/badges/3/editor\"},{\"badge_id\":1102,\"rank\":\"bronze\",\"name\":\"x86\",\"award_count\":13,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/1102/x86\"},{\"badge_id\":1103,\"rank\":\"bronze\",\"name\":\"dynamics-crm\",\"award_count\":2,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/1103/dynamics-crm\"},{\"badge_id\":1104,\"rank\":\"bronze\",\"name\":\"watir\",\"award_count\":4,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/1104/watir\"},{\"badge_id\":1105,\"rank\":\"bronze\",\"name\":\"plone\",\"award_count\":11,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/1105/plone\"},{\"badge_id\":1106,\"rank\":\"bronze\",\"name\":\"android-emulator\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/1106/android-emulator\"},{\"badge_id\":1107,\"rank\":\"bronze\",\"name\":\"joomla\",\"award_count\":4,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/1107/joomla\"},{\"badge_id\":1108,\"rank\":\"bronze\",\"name\":\"Vox Populi\",\"award_count\":5725,\"badge_type\":\"named\",\"link\":\"http://stackoverflow.com/badges/1108/vox-populi\"},{\"badge_id\":1109,\"rank\":\"bronze\",\"name\":\"verilog\",\"award_count\":3,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/1109/verilog\"},{\"badge_id\":2437,\"rank\":\"bronze\",\"name\":\"winrt-xaml\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2437/winrt-xaml\"},{\"badge_id\":2438,\"rank\":\"bronze\",\"name\":\"xml-parsing\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2438/xml-parsing\"},{\"badge_id\":2439,\"rank\":\"bronze\",\"name\":\"emulator\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2439/emulator\"},{\"badge_id\":2440,\"rank\":\"bronze\",\"name\":\"struts\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2440/struts\"},{\"badge_id\":2441,\"rank\":\"bronze\",\"name\":\"bean-validation\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2441/bean-validation\"},{\"badge_id\":2443,\"rank\":\"bronze\",\"name\":\"doctype\",\"award_count\":2,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2443/doctype\"},{\"badge_id\":2446,\"rank\":\"bronze\",\"name\":\"bufferedimage\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2446/bufferedimage\"},{\"badge_id\":2451,\"rank\":\"bronze\",\"name\":\"openlaszlo\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2451/openlaszlo\"},{\"badge_id\":2452,\"rank\":\"bronze\",\"name\":\"dom-traversal\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2452/dom-traversal\"},{\"badge_id\":2453,\"rank\":\"bronze\",\"name\":\"access-modifiers\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2453/access-modifiers\"},{\"badge_id\":2455,\"rank\":\"bronze\",\"name\":\"tostring\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2455/tostring\"},{\"badge_id\":2456,\"rank\":\"bronze\",\"name\":\"sdk\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2456/sdk\"},{\"badge_id\":2457,\"rank\":\"bronze\",\"name\":\"ios6\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2457/ios6\"},{\"badge_id\":2458,\"rank\":\"bronze\",\"name\":\"contains\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2458/contains\"},{\"badge_id\":2460,\"rank\":\"bronze\",\"name\":\"mkannotation\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2460/mkannotation\"},{\"badge_id\":2461,\"rank\":\"bronze\",\"name\":\"jax-rs\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2461/jax-rs\"},{\"badge_id\":2463,\"rank\":\"bronze\",\"name\":\"facebook-php-sdk\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2463/facebook-php-sdk\"},{\"badge_id\":2464,\"rank\":\"bronze\",\"name\":\"streamwriter\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2464/streamwriter\"},{\"badge_id\":2465,\"rank\":\"bronze\",\"name\":\"jtree\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2465/jtree\"}]"
		 * ; Type collectionType = new
		 * TypeToken<Collection<Item>>(){}.getType(); Collection<Item> items =
		 * gson.fromJson(sample2, collectionType); System.out.println(items);
		 * 
		 * // deserialize the complete structure String sample3 =
		 * "{\"items\":[{\"badge_id\":1,\"rank\":\"bronze\",\"name\":\"Teacher\",\"award_count\":407685,\"badge_type\":\"named\",\"link\":\"http://stackoverflow.com/badges/1/teacher\"},{\"badge_id\":2,\"rank\":\"bronze\",\"name\":\"Student\",\"award_count\":541987,\"badge_type\":\"named\",\"link\":\"http://stackoverflow.com/badges/2/student\"},{\"badge_id\":3,\"rank\":\"bronze\",\"name\":\"Editor\",\"award_count\":413414,\"badge_type\":\"named\",\"link\":\"http://stackoverflow.com/badges/3/editor\"},{\"badge_id\":1102,\"rank\":\"bronze\",\"name\":\"x86\",\"award_count\":13,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/1102/x86\"},{\"badge_id\":1103,\"rank\":\"bronze\",\"name\":\"dynamics-crm\",\"award_count\":2,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/1103/dynamics-crm\"},{\"badge_id\":1104,\"rank\":\"bronze\",\"name\":\"watir\",\"award_count\":4,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/1104/watir\"},{\"badge_id\":1105,\"rank\":\"bronze\",\"name\":\"plone\",\"award_count\":11,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/1105/plone\"},{\"badge_id\":1106,\"rank\":\"bronze\",\"name\":\"android-emulator\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/1106/android-emulator\"},{\"badge_id\":1107,\"rank\":\"bronze\",\"name\":\"joomla\",\"award_count\":4,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/1107/joomla\"},{\"badge_id\":1108,\"rank\":\"bronze\",\"name\":\"Vox Populi\",\"award_count\":5725,\"badge_type\":\"named\",\"link\":\"http://stackoverflow.com/badges/1108/vox-populi\"},{\"badge_id\":1109,\"rank\":\"bronze\",\"name\":\"verilog\",\"award_count\":3,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/1109/verilog\"},{\"badge_id\":2437,\"rank\":\"bronze\",\"name\":\"winrt-xaml\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2437/winrt-xaml\"},{\"badge_id\":2438,\"rank\":\"bronze\",\"name\":\"xml-parsing\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2438/xml-parsing\"},{\"badge_id\":2439,\"rank\":\"bronze\",\"name\":\"emulator\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2439/emulator\"},{\"badge_id\":2440,\"rank\":\"bronze\",\"name\":\"struts\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2440/struts\"},{\"badge_id\":2441,\"rank\":\"bronze\",\"name\":\"bean-validation\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2441/bean-validation\"},{\"badge_id\":2443,\"rank\":\"bronze\",\"name\":\"doctype\",\"award_count\":2,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2443/doctype\"},{\"badge_id\":2446,\"rank\":\"bronze\",\"name\":\"bufferedimage\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2446/bufferedimage\"},{\"badge_id\":2451,\"rank\":\"bronze\",\"name\":\"openlaszlo\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2451/openlaszlo\"},{\"badge_id\":2452,\"rank\":\"bronze\",\"name\":\"dom-traversal\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2452/dom-traversal\"},{\"badge_id\":2453,\"rank\":\"bronze\",\"name\":\"access-modifiers\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2453/access-modifiers\"},{\"badge_id\":2455,\"rank\":\"bronze\",\"name\":\"tostring\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2455/tostring\"},{\"badge_id\":2456,\"rank\":\"bronze\",\"name\":\"sdk\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2456/sdk\"},{\"badge_id\":2457,\"rank\":\"bronze\",\"name\":\"ios6\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2457/ios6\"},{\"badge_id\":2458,\"rank\":\"bronze\",\"name\":\"contains\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2458/contains\"},{\"badge_id\":2460,\"rank\":\"bronze\",\"name\":\"mkannotation\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2460/mkannotation\"},{\"badge_id\":2461,\"rank\":\"bronze\",\"name\":\"jax-rs\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2461/jax-rs\"},{\"badge_id\":2463,\"rank\":\"bronze\",\"name\":\"facebook-php-sdk\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2463/facebook-php-sdk\"},{\"badge_id\":2464,\"rank\":\"bronze\",\"name\":\"streamwriter\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2464/streamwriter\"},{\"badge_id\":2465,\"rank\":\"bronze\",\"name\":\"jtree\",\"award_count\":1,\"badge_type\":\"tag_based\",\"link\":\"http://stackoverflow.com/badges/2465/jtree\"}],\"quota_remaining\":289,\"quota_max\":300,\"has_more\":true}"
		 * ; Object result = gson.fromJson(sample3, Result.class);
		 * System.err.println(result);
		 */
	}
}
