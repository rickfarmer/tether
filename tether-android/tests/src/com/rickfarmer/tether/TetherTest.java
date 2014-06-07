package com.rickfarmer.tether;

import android.test.ActivityInstrumentationTestCase;

/**
 * This is a simple framework for a test of an Application.  See
 * {@link android.test.ApplicationTestCase ApplicationTestCase} for more information on
 * how to write and extend Application tests.
 * <p/>
 * To run this test, you can type:
 * adb shell am instrument -w \
 * -e class com.rickfarmer.tether.TetherTest \
 * com.rickfarmer.tether.tests/android.test.InstrumentationTestRunner
 */
public class TetherTest extends ActivityInstrumentationTestCase<Tether> {

    public TetherTest() {
        super("com.rickfarmer.tether", Tether.class);
    }

}
