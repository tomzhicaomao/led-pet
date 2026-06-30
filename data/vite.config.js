// Dev proxy: `npm run dev` serves frontend on localhost with API forwarded to ESP32
// Usage: Set ESP32_IP env var or edit target below
export default {
  server: {
    port: 5173,
    proxy: {
      '/api': {
        target: process.env.ESP32_IP || 'http://192.168.4.1',
        changeOrigin: true,
      }
    }
  }
}
