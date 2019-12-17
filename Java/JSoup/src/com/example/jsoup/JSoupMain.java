package com.example.jsoup;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Proxy;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

class Product {
    private final String title;
    private final String url;
    private final Map<String, String> attributes = new HashMap<>();

    public Product(String url, String title) {
        this.url = url;
        this.title = title;
    }

    public final void addAttribute(String key, String value) {
        attributes.put(key, value);
        
    }

    public final Map<String, String> getAttributes() {
        return attributes;
    }

    public String toString() {
        return String.format("Product[url=%s, title=%s]", url, title);
    }
}


class StringUtils {

    public static String strip(String self, String suffix) {
        if (self.endsWith(suffix)) {
            self = self.substring(0,  self.length() - suffix.length());
        }
        return self;
    }
    
    
    public static String rightOf(String self, String sep) {
        int idx = self.lastIndexOf(sep);
        if (idx >= 0) {
            self = self.substring(idx + sep.length());
        }
        return self;
    }
}


class ProductCategory {
    private String name;
    private String url;
    private List<Product> products = new ArrayList<>();
    private AttrKeyMapping akm = new AttrKeyMapping();

    public ProductCategory(String name, String url) {
        this.name = name;
        this.url = url;
    }

    public String toString() {
        return String.format("ProductCategory[name=\"%s\", url=%s]", name, url);
    }

    public void getProducts() {
        String prodCat = StringUtils.strip(url, ".html");
        prodCat = StringUtils.rightOf(prodCat, "-o");
        String prodCatUrl = prodCat.substring(0, 3) + "/" + prodCat;
        String prodListUrl = "https://www.conrad.de/de/_jcr_content/recommendations.html/content/conrad-de/b2c/de/catalog/articlegroups/" + prodCatUrl; 

        // https://www.conrad.de/etc/clientcontext/conrad/content/jcr:content/stores.init.js?path=%2Fcontent%2Fconrad-de%2Fb2c%2Fde%2Fcatalog%2Farticlegroups%2F240%2F2409001&cq_ck=1484033951113
        // https://www.conrad.de/de/_jcr_content/recommendations.html/content/conrad-de/b2c/de/catalog/articlegroups/240/2409001
        // https://www.conrad.de/de/_jcr_content/recommendations.html/content/conrad-de/b2c/de/catalog/articlegroups/041/0418021
        try {
            // System.err.println(prodListUrl);
            Document doc = Jsoup.connect(prodListUrl).proxy(JSoupMain.proxy).get();

            Elements prodLinks = doc.select("a.ccpProductBlock__link");
            for (Element e : prodLinks) {
                String prodUrl = "https://www.conrad.de" + e.attr("href");
                
                Document prodDoc = Jsoup.connect(prodUrl).proxy(JSoupMain.proxy).get();
                Elements prodTitle = prodDoc.select("h1.ccpProductDetail__title__text");
                Elements prodSku = prodDoc.select("span[itemprop=sku]");
                Elements prodMpn = prodDoc.select("span[itemprop=mpn]");
                Elements prodGtin = prodDoc.select("span[itemprop=gtin13]");
                Elements prodCurrency = prodDoc.select("meta[itemprop=priceCurrency]");
                Elements prodPrice = prodDoc.select("meta[itemprop=price]");
                
                Elements techData = prodDoc.select("div.tech-data > dl");
                Product prod = new Product(prodUrl, prodTitle.text());
                prod.addAttribute("SKU", prodSku.text().trim());
                prod.addAttribute("MPN", prodMpn.text().trim());
                prod.addAttribute("GTIN", prodGtin.text().trim());
                prod.addAttribute("PRICE", prodPrice.attr("content") + " " + prodCurrency.attr("content"));
                for (Element td : techData) {
                    Elements name = td.select("dt div");
                    Elements value = td.select("dd");
                    String specKey = name.text().trim().toLowerCase();
                    
                    Optional<String> mappedKey = akm.getAttrKeyMapping(specKey);
                    if (mappedKey.isPresent()) {
                        specKey = mappedKey.get();
                        String specValue = value.text().trim();
                        prod.addAttribute(specKey, specValue);
                    } else {
                        System.err.println("WARNING: NO ATTRIBUTE NAME MAPPING FOR " + specKey);
                    }

                }
                System.err.println("          " + prod);
                prod.getAttributes().forEach( (k, v) -> System.err.printf("            %s=%s%n", k, v));
System.exit(1);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        System.exit(0);
    }
}


class Category {
    private String name;
    private String url;

    public Category(String name, String url) {
        this.name = name;
        this.url = url;
    }
    
    public String toString() {
        return String.format("Category[name=\"%s\", url=%s]", name, url);
    }

    public void getSubCategories() {
        try {
            Document doc = Jsoup.connect(url).proxy(JSoupMain.proxy).get();

            Elements categoryList = doc.select("ul.category-list");

            Elements categories = categoryList.select("> li");
            for (Element category : categories) {
                Elements categoryLink = category.select("a.category-list__item__container__title");
                Elements catName = categoryLink.select("span.category-list__item__container__title__content__label");

                String url = categoryLink.attr("href");
                String name = catName.text();

                Category cat = new Category(name, /*baseUrl + */ url);

                System.err.println("     " + cat);

/**********************************/
                Elements subCategoryList = category.select("ul.category-list__item__sub__container__list.columns-list");
                Elements subCategories = subCategoryList.select("> li");
                for (Element subCategory : subCategories) {
                    Elements subCategoryLink = subCategory.select("a.category-list__item__sub__container__list__item__link");
                    Elements subCatName = subCategoryLink.select("span.category-list__item__sub__container__list__item__link__label");

                    String subUrl = subCategoryLink.attr("href");
                    String subName = subCatName.text();

                    ProductCategory prodCat = new ProductCategory(subName, JSoupMain.baseUrl + subUrl);
                    System.err.println("        " + prodCat);
                    
                    prodCat.getProducts();
System.exit(0);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}


public class JSoupMain {

    static Proxy proxy = new Proxy(
            Proxy.Type.HTTP,
            InetSocketAddress.createUnresolved("10.254.202.93", 80)
    );

    static String baseUrl;

    public static void main(String[] args) {
        if (args.length == 0) {
            System.exit(1);
        }

        new JSoupMain().crawl(args[0]);
    }

    private void crawl(String startWith) {
        baseUrl = startWith;

        try {
//            System.setProperty("http.proxyHost", "1.2.3.4");
//            System.setProperty("https.proxyHost", "1.2.3.4");
//            System.setProperty("http.proxyPort", "80");
//            Document doc = Jsoup.connect(args[0]).get();

            Document doc = Jsoup.connect(startWith).proxy(proxy).get();
            Elements categoryDiv =  doc.select("div.parsys_column.columns-5-20-20-20-20-20");
            categoryDiv.select("div.parbase.image.section");

            //Elements categories = categoryDiv.select("div.parbase.image.section");
            Elements categories = categoryDiv.select("a");
            int i = 1;
            for (Element category : categories) {
                String url = category.attr("href");
                Elements categoryImg = category.select("img");
                String name = categoryImg.attr("title");
                Category cat = new Category(name, baseUrl + url);

                System.err.printf("%d. %s%n", i++, cat);
                cat.getSubCategories();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
