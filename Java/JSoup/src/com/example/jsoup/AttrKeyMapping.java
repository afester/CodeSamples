package com.example.jsoup;

import java.util.HashMap;
import java.util.Map;
import java.util.Optional;

public class AttrKeyMapping {

    private Map<String, String> map = new HashMap<>();

    public AttrKeyMapping() {
        map.put("gtin", "GTIN");
        map.put("leistungsaufnahme (max.)",     "MAX_POWER_CONS");
        map.put("betriebsspannung",             "SUPPLY_VOLTAGE");
        map.put("druckschicht dicke (max.)",    "PRINTLAYER_THICKNESS_MAX");
        map.put("software",                     "SOFTWARE");
        map.put("druck-höhe (y) max.",          "PRINT_HEIGHT_MAX");
        map.put("druckschicht dicke (min.)",    "PRINTLAYER_THICKNESS_MIN");
        map.put("gewicht",                      "WEIGHT");
        map.put("kategorie",                    "CATEGORY");
        map.put("druck-tiefe (z) max.",         "PRINT_DEPTH_MAX");
        map.put("gehäuse-material",             "CASE_MATERIAL");
        map.put("mpn",                          "MPN");
        map.put("druck-breite (x) max.",        "PRINT_WIDTH_MAX");
        map.put("farbe",                        "COLOR");
        map.put("extruder-typ",                 "EXTRUDER_TYPE");
        map.put("druckbett-eigenschaften",      "PRINTBED_PROPERTIES");
        map.put("price",                        "PRICE");
        map.put("düsen-ø",                      "NOZZLE_DIAMETER");
        map.put("sku",                          "SKU");
        map.put("unterstütztes druckmaterial",  "PRINT_MEDIA");
        map.put("besonderheiten",               "SPEZIALITIES");
        map.put("schnittstellen",               "INTERFACES");
        map.put("ausführung",                   "MODEL");
    }

    public Optional<String> getAttrKeyMapping(String origin) {
        return Optional.ofNullable(map.get(origin));
    }
}
