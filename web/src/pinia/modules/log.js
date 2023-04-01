import { defineStore } from "pinia";
import { LogMessage } from "@/api/log";
import { ref } from "vue";

export const useLogStore = defineStore('log', () => {
    const logs = ref(null)
    const GetLogMessage = async () => {
        try {
            const res = await LogMessage()
            if (res.data.status_code == 0) {
                logs.value = res.data.logs
                return true
            }
            return false
            
        } catch (err) {
            console.log(err)
            return false
        }
    }
    return {
        logs,
        GetLogMessage
    }
}, {
    persist: true
})