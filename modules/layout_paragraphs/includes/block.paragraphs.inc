<?php
/**
 * @file
 * Layout Paragraphs block will eventually allow adding a Paragraph to content
 * via the Layout Paragraphs module.
 */
class LayoutParagraphsBlock extends Block {
  /**
   * {@inheritdoc}
   */
  function getTitle() {
    return !empty($this->settings['title']) ? check_plain($this->settings['title']) : t('Welcome to Backdrop CMS!');
  }

  /**
   * {@inheritdoc}
   */
  function getAdminTitle() {
    if (!empty($this->settings['admin_label'])) {
      return check_plain($this->settings['admin_label']);
    }

    return t('Welcome to Backdrop CMS!');
  }

  /**
   * {@inheritdoc}
   */
  function getAdminPreview() {
    if (!empty($this->settings['admin_description'])) {
      return filter_xss($this->settings['admin_description']);
    }

    return t('Welcome message, and links to getting-started tasks.');
  }

  /**
   * {@inheritdoc}
   */
  function getContent() {
    $build['tbd'] = array(
      '#type' => 'help',
      '#markup' => t('Layout Paragraphs block is not yet implemented.'),
    );
    return $build;
  }

  /**
   * Checks access to paths and returns an HTML list of links.
   *
   * @param array $links
   *   An array with each key being a path and each value being a label.
   *
   * @return array
   *   An array of HTML anchor tags for items the user may access.
   */
  private function filterAvailableLinks(array $links) {
    $html_links = array();
    foreach ($links as $path => $label) {
      if (url_is_external($path) || $path === '<front>') {
        if ($path !== '<front>') {
          $label .= '<span class="dashboard-external"></span>';
        }
        $access = TRUE;
      }
      else {
        $router_item = menu_get_item($path);
        $access = $router_item && $router_item['access'];
      }
      if ($access) {
        $html_links[] = l($label, $path, array('html' => TRUE));
      }
    }
    return $html_links;
  }
}
