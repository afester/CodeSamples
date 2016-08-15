package com.example.java8;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.function.Consumer;

public class ByteAdditionBenchmark {
    private void start() {
        int[] sizes = {
            700_000,
            1_000,
            10_000,
            25_000,
            50_000,
            100_000,
            200_000,
            300_000,
            400_000,
            500_000,
            600_000,
            700_000,
        };

        for (int size : sizes) {
            List<byte[]> arrays = createByteArrays(size);
            //warmup
            arrays.forEach(this::byteArrayCheck);

            benchmark(arrays, this::byteArrayCheck, "byteArrayCheck");
        }
    }

    private void benchmark(final List<byte[]> arrays, final Consumer<byte[]> method, final String name) {
        long start = System.nanoTime();
        arrays.forEach(method);
        long end = System.nanoTime();
        double nanosecondsPerIteration = (end - start) * 1d / arrays.size();
        System.out.println("Benchmark: " + name + " / iterations: " + arrays.size() + " / time per iteration: " + nanosecondsPerIteration + "ns");
    }

    private List<byte[]> createByteArrays(final int amount) {
        Random random = new Random();
        List<byte[]> resultList = new ArrayList<>();
        for (int i = 0; i < amount; i++) {
            byte[] byteArray = new byte[4096];
            byteArray[random.nextInt(4096)] = 1;
            resultList.add(byteArray);
        }
        return resultList;
    }

    private boolean byteArrayCheck(final byte[] array) {
        long sum = 0L;
        for (byte b : array) {
            sum += b;
        }
        return (sum == 0);
    }

    public static void main(String[] args) {
        new ByteAdditionBenchmark().start();
    }
}