/*
 * Copyright (c) 2007, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *
 * $Id: cputest.c,v 1.19 2007/10/24 22:17:46 nfi Exp $
 *
 * -----------------------------------------------------------------
 *
 * Author  : Adam Dunkels, Joakim Eriksson, Niclas Finne
 * Created : 2006-03-07
 * Updated : $Date: 2007/10/24 22:17:46 $
 *           $Revision: 1.19 $
 */

#include "msp430setup.h"
#include <stdio.h>
#include <string.h>

/* From Adams test-suite */
#define TEST(...) if(__VA_ARGS__) {					 \
                    printf("OK: " #__VA_ARGS__ " passed at %s:%d\n", __FILE__,__LINE__); \
                  } else {						 \
                    printf("FAIL: " #__VA_ARGS__ " failed at %s:%d\n", __FILE__,__LINE__); \
                  }

#define TEST2(text,...) if(__VA_ARGS__) {					 \
                    printf("OK: " #text " passed at %s:%d\n", __FILE__,__LINE__); \
                  } else {						 \
                    printf("FAIL: " #text " failed at %s:%d\n", __FILE__,__LINE__); \
                  }

#define assertTrue(...) TEST(__VA_ARGS__)
#define assertFalse(...) TEST(!(__VA_ARGS__))

#define assertTrue2(text,...) TEST2(text,__VA_ARGS__)
#define assertFalse2(text,...) TEST2(text,!(__VA_ARGS__))

static int testzero(int hm)
{
  return hm > 0;
}

static int caseID = 0;

static void initTest() {
  caseID = 0;
}

static void testCase(char *description) {
  caseID++;
  printf("-------------\n");
  printf("TEST %d: %s\n", caseID, description);
}

static void testSimple() {
  int a,b,c;
  a = 1; b = 2; c = 4;
  testCase("Arithmetic Operations");

  assertTrue((a << b) == 4);
  assertTrue((c >> a) == 2);

  assertFalse(0 > 0);
  assertFalse(a > b);

  assertFalse(testzero(0));
}

static void testIntegers() {
  int a,b,c;
  int t[3];
  testCase("Integer Operations");
  a = 1; b = 2; c = -42;
  t[0] = 1;
  t[1] = 2;
  t[2] = 3;
  assertTrue(a == 1);
  assertTrue((b + c) == -40);
  assertTrue(t[0] == 1);
  assertTrue(t[1] == 2);
  assertTrue(t[t[0]] == 2);
  assertTrue((a+b) == 3);
  assertTrue((b-a) == 1);
  assertTrue((a-b) == -1);
  assertTrue((a*b) == 2);
  assertTrue(a > 0);
  assertTrue(b > a);
  assertTrue(b != a);
  assertTrue((a ^ b) == 3);
  assertTrue((a ^ 4) == 5);
  assertTrue((a ^ a) == 0);
  assertFalse(a > b);

  a = 15;
  b = 17;
  assertTrue((a & ~b) == 14);
}

static void testFloats() {
  int i;
  float f;
  i = 2;
  f = 0.5;
  testCase("Float Operations");
  assertTrue((i * f) == 1);
  i++;
  assertTrue((i * f) == 1.5);
}

static void testStrings() {
  char buf[10];

  testCase("String Operations");
  sprintf(buf, "test");
  assertTrue2("test => test", strcmp(buf, "test") == 0);
  sprintf(buf, "%c", 'a');
  assertTrue2("buf == 'a'", strcmp(buf, "a") == 0);
}

/*--------------------------------------------------------------------------*/

static int id(int first, ...)
{
  va_list marker;
  va_start( marker, first );
  first = va_arg( marker, int);
  va_end( marker );
  return first;
}

static void testFunctions() {
  volatile int i;

  testCase("Functions");
  i = 47;

/*   printf("i=%d i+1=%d id(i)=%d\n", i, i + 1, id(0, i)); */

  assertTrue(i == 47);
  assertTrue(id(0,47) == 47);
  assertTrue(id(0,i) == 47);
}

/*--------------------------------------------------------------------------*/

static void testModulo() {
  int c,i;
  c = 1;
  testCase("Modulo");
  do {
    i = 13;

    i = i % 14;
/*     printf("(%d,%d,%d)\n",i, (i % 14), i==1); */
/*     printf("(%d,%d,%d)\n",i, (i % 14), i==1); */
    if (i < 0) printf("%d\n",i);


    c = c + 1;
  } while (c < 3);

/*     if((i % 5) == 0) { */
/*       assertTrue(i != 1); */
/*     } */
}

/*--------------------------------------------------------------------------*/

/* Bit field tests */

static struct {
  char green:4, yellow:4, red:4;
} leds;

static struct {
  char green:2, yellow:2, red:2;  /* These bit fields should really be
				     only one bit wide, but that
				     crashed gcc... */
} invert;

#include <io.h>
#ifndef BV
#define BV(n) (1 << (n))
#endif

void testBis() {
  P4DIR |= BV(2) | BV(5) | BV(6);
  assertTrue(P4DIR == 100);
}

static void testBitFields() {
  static int pelle = 0;

  testCase("Bit field Operations");

  P4DIR = 0;
  testBis();

  leds.green = 1;
  leds.yellow = 1;
  leds.red = 1;

  assertTrue(leds.green > 0);
  assertTrue(leds.yellow > 0);
  assertTrue(leds.red > 0);

  leds.green -= 1;
  assertTrue(leds.green == 0);

  pelle = 0x4711;

  invert.green = 1;
  invert.yellow = 1;
  invert.red = 1;

  assertTrue(invert.green > 0);
  assertTrue(invert.yellow > 0);
  assertTrue(invert.red > 0);

  invert.green ^= 1;
  assertTrue(invert.green == 0);

}


/*--------------------------------------------------------------------------*/

int
main(void)
{
  msp430_setup();

  initTest();

/*   testSimple(); */
/*   testIntegers(); */
/*   testFloats(); */
/*   testStrings(); */
/*   testBitFields(); */
/*   testFunctions(); */
  testModulo();

  printf("EXIT\n");
  return 0;
}
