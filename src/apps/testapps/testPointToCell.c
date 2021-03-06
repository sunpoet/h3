/*
 * Copyright 2016-2018, 2020 Uber Technologies, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/** @file
 * @brief tests H3 function `pointToCell`
 *
 *  usage: `testPointToCell`
 *
 *  The program reads lines containing H3 indexes and lat/lon  pairs from
 *  stdin until EOF is encountered. For each input line, it calls `pointToCell`
 *  to convert the input lat/lon to an H3 index, and then validates the
 *  index against the original input index.
 */

#include <stdio.h>
#include <stdlib.h>

#include "geoPoint.h"
#include "h3Index.h"
#include "test.h"
#include "utility.h"

static void assertExpected(H3Index h1, const GeoPoint* g1) {
    // convert lat/lon to H3 and verify
    int res = H3_EXPORT(getResolution)(h1);
    H3Index h2;
    t_assertSuccess(H3_EXPORT(pointToCell)(g1, res, &h2));
    t_assert(h1 == h2, "got expected pointToCell output");
}

int main(int argc, char* argv[]) {
    // check command line args
    if (argc > 1) {
        fprintf(stderr, "usage: %s\n", argv[0]);
        exit(1);
    }

    // process the indexes and lat/lons on stdin
    char buff[BUFF_SIZE];
    char h3Str[BUFF_SIZE];
    while (1) {
        // get an index from stdin
        if (!fgets(buff, BUFF_SIZE, stdin)) {
            if (feof(stdin))
                break;
            else
                error("reading input from stdin");
        }

        double latDegs, lonDegs;
        if (sscanf(buff, "%s %lf %lf", h3Str, &latDegs, &lonDegs) != 3)
            error("parsing input (should be \"H3Index lat lon\")");

        H3Index h3;
        h3 = H3_EXPORT(stringToH3)(h3Str);

        GeoPoint coord;
        setGeoDegs(&coord, latDegs, lonDegs);

        assertExpected(h3, &coord);
    }
}
