<?php

/**
 * @file
 */

 /**
 * Form constructor for the Paragraphs layout configuration form.
 *
 * @param array $form
 *   An associative array containing the structure of a portion of the form.
 * @param array $form_state
 *   A keyed array containing the current state of the form.
 * @param array $field
 *   The field structure.
 * @param array $instance
 *   The field instance.
 *
 * @see layout_paragraphs_menu()
 * @see layout_paragraphs_edit_content_validate() @todo
 * @see layout_paragraphs_edit_content_submit() @todo
 * @ingroup forms
 */
function layout_paragraphs_edit_content($form, &$form_state, $field = array(), $instance = array()) {

  if ($instance['widget']['type'] == 'layout_paragraphs') {
    form_load_include($form_state, 'inc', 'layout_paragraphs', 'layout_paragraphs.admin');
    form_load_include($form_state, 'inc', 'layout_paragraphs', 'layout_paragraphs.context.admin');
    form_load_include($form_state, 'inc', 'layout', 'layout.admin');

    $form['#attached']['js'][] = backdrop_get_path('module', 'layout') . '/js/layout.admin.js';
    // $form['#attached']['css'][] = backdrop_get_path('module', 'mini_layouts') . '/css/mini-layouts.admin.css';

    $available_templates = $instance['widget']['settings']['available_layout_templates'];
    $all_templates = layout_paragraphs_template_load_all();
    if (!empty($available_templates)) {
      //($instance['widget']['settings']['available_layout_templates']);
      foreach ($available_templates as $template) {
        if (!empty($all_templates[$template])) {
          $options[$template] = $all_templates[$template]['title'];
        }
      }
      if (count($options) > 1) {
        $form['layout_template'] = array(
          '#type' => 'select',
          '#title' => t('Layout template'),
          '#description' => t('Which layout template should be used?'),
          '#options' => $options,
          '#default_value' => 'default',
          '#required' => TRUE,
        );
        $form['markup'] = array(
          '#markup' => '<p>' . t('Select template from a list of available templates.') . '</p>',
        );
      }
      else {
        $form['layout_template'] = array(
          '#type' => 'hidden',
          '#title' => t('Layout template'),
          '#description' => t('Which layout template should be used?'),
          '#value' => (!empty($template)) ? $template : 'default',
          '#required' => TRUE,
        );
        $form['markup'] = array(
          '#markup' => '<p>' . t('Only one choice possible, so we will use it.') . '</p>',
        );
      }

    }
    else {
      $form['markup'] = array(
        '#markup' => '<p>' . t('No layout templates are available for this field. Using default.') . '</p>',
      );
    }
    $form['layout_paragraph']['#markup'] = 'RENDER LAYOUT HERE';

    $layout = layout_load('default', TRUE, 'paragraphs');
    $renderer = layout_create_renderer('layout_paragraphs', $layout);
    $form['test']['#markup'] = $renderer->render();

    return $form;
  }
}


/**
 * Administrative form for layout settings.
 */
function layout_paragraphs_settings_page($form, &$form_state) {
  $header = array(
    'layout_paragraphs_template_info' => array(
      'data' => t('Template')),
    'type' => array(
      'data' => t('Type'),
      'sort' => 'desc',
      'class' => array(RESPONSIVE_PRIORITY_LOW),
    ),
    'operations' => t('Operations'),
  );

  backdrop_set_title(t('Layout Paragraphs settings'));
  $excluded = config_get('paragraphs.settings', 'excluded_layouts');

  $rows = array();
  $options = array();
  $default = array();
  $form_state['hidden_templates'] = array();
  $all_templates = layout_paragraphs_template_load_all();

  /*
  foreach ($all_templates as $template_name => $template) {
    // Build the default values list.
    $is_default = FALSE;
    (empty($template->hidden)) ? $template->hidden = 0 : $template->hidden = 1;
    $template_info = $template->info;
    if (is_array($excluded) && !in_array($template_name, $excluded)) {
      $is_default = $default[$template_name] = TRUE;
    }

    // Build the list of possible options, either enabled or not hidden.
    if ($is_default || !$template->hidden) {
      $rows[$template_name] = array(
        'layout_paragraphs_template_info' => theme('layout_paragraphs_template_info', array(
          'template_info' => $template_info,
        )),
        'type' => t('Paragraphs'),
      );
      $rows[$template_name]['operations']['data'] = array(
        '#type' => 'dropbutton',
        '#links' => _layout_paragraphs_settings_get_group_operations($template_info, $is_default),
      );
    }
    // For disabled and hidden templates, save the fact they are disabled.
    elseif ($template->hidden) {
      $form_state['hidden_templates'][] = $template_name;
    }
  }
  */

  $form['#attached']['css'][] = backdrop_get_path('module', 'layout') . '/css/layout.admin.css';
  $form['templates'] = array(
    '#type' => 'tableselect',
    '#js_select' => FALSE,
    '#header' => $header,
    '#options' => $rows,
    '#empty' => t('There are no Layout Paragraphs templates.'),
    '#default_value' => $default,
    '#attributes' => array('class' => array('layout-list')),
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Validation for Layout Paragraphs settings page.
 *
 * Ensures that active layouts are not disabled while in use.
 */
function layout_paragraphs_settings_page_validate(&$form, &$form_state) {
  $options = array_keys($form_state['values']['templates']);
  $included = array_values($form_state['values']['templates']);
  $excluded = array_unique(array_diff($options, $included));
  $form_state['excluded_layouts'] = array_values(array_merge($excluded, $form_state['hidden_templates']));

  // Ensure all layouts are not disabled.
  if (empty($form_state['values']['templates'])) {
    form_set_error('templates', t('At least one layout template must be enabled.'));
  }

  // Check which layout templates are in use.
  // @todo This can just load all Paragraphs fields that use the Layouts widget
  // and check which templates are in use from the settings there.
  $all_templates = layout_paragraphs_template_load_all();
  foreach ($all_templates as $template_name => $template) {
    if (in_array($template_name, $excluded)) {
      form_set_error('templates][' . $template_name, t('The "@layout" layout template is currently in use and may not be disabled.', array('@layout' => $template->title)));
    }
  }
}

/**
 * Submit handler for Layout Paragraphs settings page.
 */
function layout_paragraphs_settings_page_submit($form, &$form_state) {
  if (!empty($form_state['excluded_layouts'])) {
    config_set('layout_paragraphs.settings', 'excluded_layouts', $form_state['excluded_layouts']);
  }
  backdrop_set_message(t('Your configuration has been saved.'));
}
