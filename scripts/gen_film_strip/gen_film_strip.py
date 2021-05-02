
"""
Generate film strip

Requires Pillow (pip install Pillow)
"""

import os
import sys
from PIL import Image

input_path = './input/'
output_path = './output/'
imagesArray = [input_path + str('%04d'%index) + '.png' for index in range(256)]

images = [Image.open(x) for x in imagesArray]
widths, heights = zip(*(i.size for i in images))

def gen_strip():
    total_width = max(widths)
    max_height = sum(heights)
    new_im = Image.new('RGBA', (total_width, max_height))
    y_offset = 0
    for im in images:
        new_im.paste(im, (0, y_offset))
        y_offset += im.size[1]
        print('Add Image "%s"..'%im.filename)
    return new_im

if not os.path.exists(output_path):
    os.makedirs(output_path)

gen_strip().save(output_path + 'knob.png')
print('Finish')