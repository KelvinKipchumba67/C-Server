document.addEventListener('DOMContentLoaded', () => {
  const toggleButton = document.querySelector('.menu-toggle');
  const navLinks = document.querySelector('.nav-links');
//ensures this code does not run until HTML is fully loaded
  if (toggleButton && navLinks) {
    toggleButton.addEventListener('click', () => {
      navLinks.classList.toggle('active');
    });

    navLinks.querySelectorAll('a').forEach((link) => {
      link.addEventListener('click', () => {
        navLinks.classList.remove('active');
      });
    });
  }

  const sliderEl = document.querySelector('.swiper');
  if (!sliderEl || typeof window.Swiper === 'undefined') return;

  // eslint-disable-next-line no-new
  new window.Swiper(sliderEl, {
    loop: true,
    speed: 650,
    effect: 'fade',
    fadeEffect: {
      crossFade: true,
    },
    navigation: {
      nextEl: '.swiper-button-next',
      prevEl: '.swiper-button-prev',
    },
    pagination: {
      el: '.swiper-pagination',
      clickable: true,
    },
    autoplay: {
      delay: 4500,
      disableOnInteraction: false,
    },
  });
});