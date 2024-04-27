// Copyright (c) 2023 by Rockchip Electronics Co., Ltd. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*-------------------------------------------
                Includes
-------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ppocr_rec.h"
#include "file_utils.h"

#define INDENT "    "

/*-------------------------------------------
                  Main Function
-------------------------------------------*/
int main(int argc, char** argv)
{
    if (argc != 3) {
        printf("%s <model_path> <image_path>\n", argv[0]);
        return -1;
    }

    const char* model_path = argv[1];
    const char* image_path = argv[2];

    int ret;
    rknn_app_context_t rknn_app_ctx;
    memset(&rknn_app_ctx, 0, sizeof(rknn_app_context_t));

    ret = init_ppocr_rec_model(model_path, &rknn_app_ctx);
    if (ret != 0) {
        printf("init_ppocr_rec_model fail! ret=%d model_path=%s\n", ret, model_path);
        return -1;
    }

    cv::Mat src_img = cv::imread(image_path);
    
    if (src_img.empty()) {
        printf("Failed to read image: %s\n", image_path);
        return -1;
    }

    ppocr_rec_result results;
    char score_result[64];

    ret = inference_ppocr_rec_model(&rknn_app_ctx, &src_img, &results);
    if (ret != 0) {
        printf("inference_ppocr_rec_model fail! ret=%d\n", ret);
        goto out;
    }

    printf("regconize result: %s, score=%f\n", results.str, results.score);

out:
    ret = release_ppocr_rec_model(&rknn_app_ctx);
    if (ret != 0) {
        printf("release_ppocr_rec_model fail! ret=%d\n", ret);
    }

    return 0;
}