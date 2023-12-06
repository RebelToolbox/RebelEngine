// SPDX-FileCopyrightText: 2023 Rebel Engine contributors
// SPDX-FileCopyrightText: 2014-2022 Godot Engine contributors
// SPDX-FileCopyrightText: 2007-2014 Juan Linietsky, Ariel Manzur
//
// SPDX-License-Identifier: MIT

package com.rebeltoolbox.rebelengine.utils;

import java.security.MessageDigest;
import java.util.Random;

public class Crypt {
    public static String md5(String input) {
        try {
            // Create MD5 Hash
            MessageDigest digest =
                java.security.MessageDigest.getInstance("MD5");
            digest.update(input.getBytes());
            byte[] messageDigest = digest.digest();

            // Create Hex String
            StringBuilder hexString = new StringBuilder();
            for (int i = 0; i < messageDigest.length; i++)
                hexString.append(Integer.toHexString(0xFF & messageDigest[i]));
            return hexString.toString();

        } catch (Exception e) {
            e.printStackTrace();
        }
        return "";
    }

    public static String createRandomHash() {
        return md5(Long.toString(createRandomLong()));
    }

    public static long createAbsRandomLong() {
        return Math.abs(createRandomLong());
    }

    public static long createRandomLong() {
        Random r = new Random();
        return r.nextLong();
    }
}
