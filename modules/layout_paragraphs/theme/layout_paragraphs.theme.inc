<?php

/**
 * Outputs information about a Layout Paragraphs template . Used in the
 * Paragraphs layout template listing page.
 *
 * @param $variables
 *   An array with a single value: "template_info", which contains information
 *   about the layout template.
 *
 * @return string
 */
function theme_layout_paragraphs_template_info($variables) {
  $template_info = $variables['template_info'];

  // Get the label and machine name.
  $label = theme('label_machine_name__layout_template', array(
    'label' => $template_info['title'],
    'machine_name' => $template_info['name'],
  ));

  $output = '<div class="layout-info">';
  $output .= $template_info['preview'];
  $output .= '<div class="layout-detail">' . $label . '</div>';
  $output .= '</div>';

  return $output;
}


/**
 * Outputs an individual option label for selecting a layout paragraph template.
 */
function theme_layout_paragraphs_template_option($variables) {
  $template_info = $variables['template_info'];

  $output = '';
  $output .= '<div class="layout-template">';
  $output .= $template_info->preview;
  //$output .= '<div class="layout-caption">' . check_plain($template_info->title) . '</div>';
  $output .= '</div>';
  return $output;
}
